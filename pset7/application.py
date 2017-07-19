from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    if request.method == "GET":
        userid = session["user_id"]
        purchases = db.execute(" select stock_symbol, SUM(num_shares) as shares from stocks where user_id = :id group by stock_symbol", id = userid)
        amount = db.execute("select cash from users where id = :id", id = userid)
        stock_details = {}
        total_price = 0
        for purchase in purchases:
            symbol = purchase["stock_symbol"];
            stock_details[symbol] = lookup(symbol)
            total_price += stock_details[symbol]["price"] * purchase["shares"]
        total_price += amount[0]["cash"]
        total_price = usd(total_price)
        return render_template("index.html", cash = usd(amount[0]["cash"]) , stocks = purchases , details = stock_details, total = total_price )
    else:
        return apology("TODO")
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")
    elif request.method == "POST":
        if request.form["symbol"] == "" or request.form["shares"] == "":
            return apology("Input should not be blank")
        elif not (request.form["shares"]).isdigit():
            return apology("Enter valid number of shares")
        elif (int(request.form["shares"]) - float(request.form["shares"])) != 0:
            return apology("Number of shares should be an integer")
        elif int(request.form["shares"]) <= 0:
            return apology("Number of share should be positive")
        else:
            stock_details = lookup(request.form["symbol"])
            if stock_details == None:
                return apology("Enter valid stock symbol")
            price = stock_details["price"] * int(request.form["shares"])
            rows = db.execute("select * from users where id = :id", id = session["user_id"])
            if rows[0]["cash"] < price:
                return apology("Transaction cannot be processed because of low balance")
            db.execute("insert into purchases (user_id, stock_symbol, num_shares, cost) values (:user_id, :symbol, :shares, :cost)",user_id = session["user_id"], symbol = request.form["symbol"], shares = int(request.form["shares"]), cost = price)
            db.execute("update users set cash = :cash where id = :id ", cash = rows[0]["cash"] - price, id = session["user_id"])
            stocks = db.execute("select * from stocks where user_id = :id and stock_symbol = :symbol", id = session["user_id"], symbol = request.form["symbol"])
            if len(stocks) == 0:
                db.execute("insert into stocks (user_id, stock_symbol, num_shares) values (:user_id, :symbol, :shares)",user_id = session["user_id"], symbol = request.form["symbol"], shares = int(request.form["shares"]))
            else:
                db.execute("update stocks set num_shares = :shares where user_id = :id and stock_symbol = :symbol", shares = int(request.form["shares"]) + stocks[0]["num_shares"], id = session["user_id"], symbol = request.form["symbol"])
            flash("Bought!")
            return redirect(url_for("index"))
    return apology("TODO")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "GET":
        purchase = db.execute("select * from purchases where user_id = :id", id = session["user_id"])
        sale = db.execute("select * from sales where user_id = :id", id = session["user_id"])
        return render_template("history.html", purchases = purchase, sales = sale)
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        stock_details = lookup(request.form["symbol"])
        if stock_details != None:
            return render_template("quoted.html",stock = stock_details)
        else:
            return apology("Invalid stock symbol")
    else:    
        return apology("TODO")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        if request.form["username"] == "" or request.form["password"] == "" or request.form["repassword"] == "":
            return apology("Input should not be blank")
        elif request.form["password"] != request.form["repassword"] :
            return apology("passwords should match")
        else:
            user = request.form["username"]
            rows = db.execute("select * from users")
            if len(rows) != 0:
                for row in rows:
                    if rows[0]["username"] == user:
                        return apology("username already exists")
            db.execute("insert into users (username, hash) values (:username, :hashie)", username = request.form["username"], hashie =  pwd_context.hash(request.form.get("password")))
            flash("Registered successfully!")
            return redirect(url_for("index"))
    else:
        return apology("TODO")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "GET":
        return render_template("sell.html")
    elif request.method == "POST":
        if request.form["symbol"] == "" or request.form["shares"] == "":
            return apology("Input should not be blank")
        elif not (request.form["shares"]).isdigit():
            return apology("Enter valid number of shares")
        elif (int(request.form["shares"]) - float(request.form["shares"])) != 0:
            return apology("Number of shares should be an integer")
        elif int(request.form["shares"]) <= 0:
            return apology("Number of share should be positive")
        else:
            user = db.execute("select * from users where id = :id", id = session["user_id"])
            stocks = db.execute("select * from stocks where user_id = :id and stock_symbol = :symbol", id = session["user_id"], symbol = request.form["symbol"])
            if len(stocks) == 0:
                return apology("You don't have enough stocks")
            else:
                if stocks[0]["num_shares"] < int(request.form["shares"]):
                    return apology("You don't have enough stocks")
                else:
                    if stocks[0]["num_shares"] == int(request.form["shares"]):
                        db.execute("delete from stocks where user_id = :id and stock_symbol = :symbol", id = session["user_id"], symbol = request.form["symbol"] )
                    else:
                        db.execute("update stocks set num_shares = :shares where user_id = :id and stock_symbol = :symbol ", shares = stocks[0]["num_shares"] - int(request.form["shares"]), id = session["user_id"], symbol = request.form["symbol"] )
            stock_details = lookup(request.form["symbol"])
            price = stock_details["price"] * int(request.form["shares"])
            db.execute("insert into sales (user_id, stock_symbol, num_shares, cost) values (:user_id, :symbol, :shares, :cost)",user_id = session["user_id"], symbol = request.form["symbol"], shares = int(request.form["shares"]), cost = price)
            db.execute("update users set cash = :cash where id = :id ", cash = user[0]["cash"] + price, id = session["user_id"])    
            flash("Sold!")
            return redirect(url_for("index"))
            
    return apology("TODO")
    
@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change password."""
    if request.method == "GET":
        return render_template("change.html")
    elif request.method == "POST":
        cp = request.form["password"]
        np = request.form["new_password"]
        rp = request.form["re_password"]
        if cp == "" or np == "" or rp == "":
            return apology("Enter all values")
        elif np != rp:
            return apology("Passwords did not match")
        else:
            rows = db.execute("select * from users where id = :id", id = session["user_id"])
            if not pwd_context.verify(cp, rows[0]["hash"]):
                return apology("Incorrect password")
            db.execute("update users set hash = :hashie where id = :id", hashie = pwd_context.hash(np), id = session["user_id"])
            flash("Password changed successfully!")
            return redirect(url_for("index"))
    return apology("TODO")

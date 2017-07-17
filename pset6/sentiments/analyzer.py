import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = positives
        self.negatives = negatives
    
    def positive(self, str):
        fp = open(self.positives, "r")
        for line in fp:
            if not line.startswith(';'):
                if line.strip() == str:
                    return 1
        return 0
    def negative(self, str):
        fp = open(self.negatives, "r")
        for line in fp:
            if not line.startswith(';'):
                if line.strip() == str:
                    return 1
        return 0


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        for word in tokens:
            if self.positive(word.lower()):
                score +=  1
            elif self.negative(word.lower()):
                score += -1
            else:
                score += 0
        return score

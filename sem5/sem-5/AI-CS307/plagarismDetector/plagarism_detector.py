import nltk
import string
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity

# Download necessary resources
nltk.download('punkt')

def compute_similarity(segment1, segment2):
    vectorizer = TfidfVectorizer()
    tfidf_matrix = vectorizer.fit_transform([segment1, segment2])
    similarity_matrix = cosine_similarity(tfidf_matrix)
    return similarity_matrix[0, 1]

def preprocess_text(text):
    sentences = nltk.sent_tokenize(text)
    normalized_sentences = []
    for sentence in sentences:
        sentence = sentence.lower()
        sentence = sentence.translate(str.maketrans('', '', string.punctuation))
        normalized_sentences.append(sentence)
    return normalized_sentences

def compute_plagiarism_score(source_segments, target_segments):
    total_similarity = 0.0
    matches = []

    for source_segment in source_segments:
        best_similarity = 0.0
        best_match = None
        
        for target_segment in target_segments:
            similarity = compute_similarity(source_segment, target_segment)
            if similarity > best_similarity:
                best_similarity = similarity
                best_match = target_segment
        
        matches.append((source_segment, best_match, best_similarity))
        total_similarity += best_similarity

    # Calculate the average similarity as the overall plagiarism score
    average_similarity = total_similarity / len(source_segments) if source_segments else 0
    return average_similarity, matches

# Example usage
source_text = "The quick brown fox jumps over the lazy dog. It was a sunny day."
target_text = "A speedy brown fox leaps over the sleepy dog. The day was bright and sunny."

source_segments = preprocess_text(source_text)
target_segments = preprocess_text(target_text)

print("Source Segments:", source_segments)
print("Target Segments:", target_segments)

# Compute the plagiarism score
plagiarism_score, matches = compute_plagiarism_score(source_segments, target_segments)

print(f"Overall Plagiarism Score: {plagiarism_score:.2f}\n")

# Output the results
for source_segment, target_segment, similarity in matches:
    print(f"Source Segment: '{source_segment}'")
    print(f"Best Match in Target: '{target_segment}'")
    print(f"Similarity Score: {similarity:.2f}\n")

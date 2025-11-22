import heapq
import nltk
import string
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity

# Download necessary resources
nltk.download('punkt')

def compute_similarity(segment1, segment2):
    # Initialize the vectorizer
    vectorizer = TfidfVectorizer()
    
    # Fit and transform the segments into TF-IDF vectors
    tfidf_matrix = vectorizer.fit_transform([segment1, segment2])
    
    # Compute the cosine similarity between the two vectors
    similarity_matrix = cosine_similarity(tfidf_matrix)
    
    # Return the similarity score (value between 0 and 1)
    return similarity_matrix[0, 1]

# Function to preprocess text: tokenize and normalize
def preprocess_text(text):
    # Tokenize the text into sentences
    sentences = nltk.sent_tokenize(text)
    
    # Normalize each sentence: lowercase and remove punctuation
    normalized_sentences = []
    for sentence in sentences:
        # Convert to lowercase
        sentence = sentence.lower()
        
        # Remove punctuation
        sentence = sentence.translate(str.maketrans('', '', string.punctuation))
        
        # Append the normalized sentence
        normalized_sentences.append(sentence)
    
    return normalized_sentences

# A* Search function to find the best match for the source segments in the target segments
def a_star_search(source_segments, target_segments):
    # Priority queue to hold the open set
    open_set = []
    
    # Initialize the open set with each segment in the source text
    for index, segment in enumerate(source_segments):
        heapq.heappush(open_set, (0, index, segment))  # (cost, index, segment)
    
    # Dictionary to keep track of the best match
    best_match = {}
    came_from = {}
    cost_so_far = {index: 0 for index in range(len(source_segments))}
    
    # Loop through the open set
    while open_set:
        current_cost, current_index, current_segment = heapq.heappop(open_set)
        
        # Calculate similarity for each target segment
        for target_index, target_segment in enumerate(target_segments):
            similarity_score = compute_similarity(current_segment, target_segment)
            cost = 1 - similarity_score  # Heuristic: the higher the similarity, the lower the cost
            
            if target_index not in best_match or cost < best_match[target_index][0]:
                best_match[target_index] = (cost, current_index, current_segment)
                came_from[target_index] = current_index
                heapq.heappush(open_set, (cost, current_index, current_segment))
                cost_so_far[target_index] = cost
    
    return best_match, came_from

# Function to reconstruct the path based on the came_from map
def reconstruct_path(came_from, start_index, end_index):
    path = [end_index]
    while end_index in came_from:
        end_index = came_from[end_index]
        path.append(end_index)
    path.reverse()
    return path

# Example usage
source_text = "The quick brown fox jumps over the lazy dog. It was a sunny day."
target_text = "The day was bright and sunny. A speedy brown fox leaps over the sleepy dog."

# Preprocess the texts
source_segments = preprocess_text(source_text)
target_segments = preprocess_text(target_text)

print("Source Segments:", source_segments)
print("Target Segments:", target_segments)




best_matches, came_from = a_star_search(source_segments, target_segments)

# Output the results
for target_index, (cost, source_index, source_segment) in best_matches.items():
    print(f"Source Segment: '{source_segments[source_index]}'")
    print(f"Target Segment: '{target_segments[target_index]}'")
    print(f"Similarity Score: {1 - cost}\n")

# Reconstruct the path (if needed)
path = reconstruct_path(came_from, 0, max(came_from.keys()))
print("Reconstructed Path:", path)
print("Matched Segments in Order:")
for index in path:
    print(f"Target Segment: '{target_segments[index]}'")

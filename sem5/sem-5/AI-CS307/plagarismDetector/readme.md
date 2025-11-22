#archive/tests

# Plagiarism Detection Using Cosine Similarity

  

This script is designed to detect plagiarism by comparing segments of a source text against a target text. It utilizes the TF-IDF (Term Frequency-Inverse Document Frequency) vectorization technique and computes cosine similarity to determine the degree of similarity between text segments.

  

## How the Code Works

  

### 1. Preprocessing the Text

The text is first tokenized into sentences and normalized by:

- Converting all characters to lowercase.

- Removing punctuation.

  

### 2. Computing Similarity

The `compute_similarity` function uses TF-IDF vectorization to transform text segments into vectors. It then calculates the cosine similarity between these vectors to determine how similar two text segments are.

  

### 3. Matching Segments

For each segment in the source text, the script finds the most similar segment in the target text. It then calculates an overall plagiarism score based on these similarities.

  

### 4. Output

The script outputs:

- The overall plagiarism score as an average similarity across all segments.

- Detailed information about which segments were matched and their corresponding similarity scores.

  

## Requirements

  

- Python 3.7 or higher

- `nltk` (Natural Language Toolkit)

- `scikit-learn`

  

## Setup and Installation

  

Follow these steps to set up and run the script in a virtual environment:

  

### 1. Create and Activate the Virtual Environment

  

Create a virtual environment named `venv`:

  

```bash

python3 -m venv venv

```

  

Activate the virtual environment:

  

- **Windows:**
```bash

  .\venv\Scripts\activate
```

- **macOS/Linux:**
```bash

  source venv/bin/activate
```

  

### 2. Install Dependencies

  

Install the required Python packages:

  

```bash

pip install -r requirements.txt

```

  


### 3. Run the Script

  

Once the virtual environment is set up and dependencies are installed, you can run the script:

  

```bash

python plagiarism_detection.py

```

  

### 4. Deactivate the Virtual Environment

  

When you're done, deactivate the virtual environment by running:

  

```bash

deactivate

```

  

## Example Usage

  

Here's a brief example of how to use the script:

  

```python

source_text = "The quick brown fox jumps over the lazy dog. It was a sunny day."

target_text = "A speedy brown fox leaps over the sleepy dog. The day was bright and sunny."

  

source_segments = preprocess_text(source_text)

target_segments = preprocess_text(target_text)

  

plagiarism_score, matches = compute_plagiarism_score(source_segments, target_segments)

  

print(f"Overall Plagiarism Score: {plagiarism_score:.2f}\n")

for source_segment, target_segment, similarity in matches:

    print(f"Source Segment: '{source_segment}'")

    print(f"Best Match in Target: '{target_segment}'")

    print(f"Similarity Score: {similarity:.2f}\n")

```
# Marble Solitaire solution


## Problem overview

  

The board is represented as 7x7 matrix in code. Player would pick one marble will make it jump over one marble by moving the selected marble by two steps in left or right or up or down direction, and the marble which got jumped upon will be removed from the grid. The goal of the game is to have only one marble in this grid which should be at centre position i.e. (3,3) position.

## Code Details

  

1. Implemented the node class same as puzzle_eight problem.

  

2. Search agent search for only valid successors, this validity checker is implemented by function named is_valid move and then if the move is valid further computations are done

  

3. After there is surety that moves are valid they are added to priority queue based on three different approaches
	1. **BFS(Best First Search) :** for this function the prioritization happened on the basis of number of marbles remaining on board
	2. **Manhattan distance :** for this function the prioritization happened on the basis of number of marbles remaining on board + sum Manhattan distance of each marble for current board 
	3. **Exponential distance :** for this function the prioritization happened on the basis of number of marbles remaining on board + sum Exponential distance of each marble for current board
4. Difference in performance in each approach 
	1. **BFS:** This explored little over 1000 nodes to reach to final state
	2. **Manhattan distance:** This explored nearly 1,40,000 nodes to reach to final state
	3. **Exponential distance:** This explored nearly 25,000 nodes to reach to final state
## Sample outputs of each approach 

### 1. BFS
```plaintext
Total number of nodes explored: 1093
Solution Found
(1, 3, 3, 3)
(2, 1, 2, 3)
(0, 2, 2, 2)
(0, 4, 0, 2)
(2, 3, 2, 1)
(2, 0, 2, 2)
(2, 4, 0, 4)
(2, 6, 2, 4)
(3, 2, 1, 2)
(0, 2, 2, 2)
(3, 0, 3, 2)
(3, 2, 1, 2)
(3, 4, 3, 2)
(3, 6, 3, 4)
(3, 4, 1, 4)
(0, 4, 2, 4)
(4, 2, 2, 2)
(1, 2, 3, 2)
(4, 0, 4, 2)
(4, 3, 4, 1)
(5, 4, 3, 4)
(4, 6, 4, 4)
(6, 2, 4, 2)
(3, 2, 5, 2)
(6, 4, 6, 2)
(6, 2, 4, 2)
(4, 1, 4, 3)
(4, 3, 4, 5)
(2, 4, 4, 4)
(4, 5, 4, 3)
(5, 3, 3, 3)
```
### 2. Manhattan Distance 
```plaintext
Total number of nodes explored: 139258
Solution Found
(1, 3, 3, 3)
(2, 1, 2, 3)
(0, 2, 2, 2)
(0, 4, 0, 2)
(4, 1, 2, 1)
(2, 4, 0, 4)
(2, 6, 2, 4)
(4, 6, 2, 6)
(4, 5, 2, 5)
(3, 2, 1, 2)
(0, 2, 2, 2)
(5, 2, 3, 2)
(3, 2, 1, 2)
(2, 0, 2, 2)
(4, 0, 2, 0)
(2, 3, 2, 1)
(2, 0, 2, 2)
(1, 2, 3, 2)
(3, 4, 1, 4)
(0, 4, 2, 4)
(3, 2, 3, 4)
(3, 4, 1, 4)
(2, 6, 2, 4)
(1, 4, 3, 4)
(4, 3, 4, 5)
(6, 4, 4, 4)
(6, 2, 6, 4)
(3, 4, 5, 4)
(6, 4, 4, 4)
(4, 5, 4, 3)
(5, 3, 3, 3)
```
### 3. Exponential Distance
```plaintext
Total number of nodes explored: 25520
Solution Found
(1, 3, 3, 3)
(2, 1, 2, 3)
(0, 2, 2, 2)
(0, 4, 0, 2)
(4, 1, 2, 1)
(3, 2, 1, 2)
(0, 2, 2, 2)
(5, 2, 3, 2)
(5, 4, 5, 2)
(6, 2, 4, 2)
(6, 4, 6, 2)
(3, 2, 5, 2)
(6, 2, 4, 2)
(3, 4, 3, 2)
(1, 4, 3, 4)
(2, 6, 2, 4)
(4, 6, 2, 6)
(2, 3, 2, 5)
(2, 6, 2, 4)
(2, 1, 2, 3)
(4, 3, 4, 1)
(4, 0, 4, 2)
(2, 0, 4, 0)
(4, 5, 4, 3)
(3, 4, 1, 4)
(4, 3, 4, 1)
(4, 0, 4, 2)
(4, 2, 2, 2)
(2, 2, 2, 4)
(1, 4, 3, 4)
(3, 5, 3, 3)
```

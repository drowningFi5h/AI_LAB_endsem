#print ('Hello World')

#--------------------------------------------

#Learning data types
#a=10 #int
#b=10.1 #float
#c="sarthak" #string : Note - always in ""
#d=sarthak # this is wrong
#print (d)

#--------------------------------------------

#Difference between = and ==
#= is used for assignment
#== is used for comparison

#--------------------------------------------

#if statememts in python and use of indentations

#a = 10

#if a==10:
#print("yes my a is 10")
#print("one more line")
#else /* : */
#print("a is not 10")
#--------------------------------------------

#Use of elif in python

#a = 17

#if a==10:
#print("a is 10")

#elif a==11:
#print("a is 11")

#elif a==12:
#print("a is 12")

#else /* : */
#print("none")

#--------------------------------------------

#Taking input from user

#Method 1 of taking input
## print("Enter a value:")
## a = input()
## print(a)

#Method 2 of taking input
#a = input("Enter the value of a:")

#Computer considers everything as string while taking INPUT FROM USER
#a = input("Enter the value of a:")
#print(type(a)) # Note that type() is used to check type of any variable
#print(a)

#Method to solve this issue
#a = eval(input("Enter the value of a:"))
#print(type(a))
#print(a)

#--------------------------------------------

#and/or in python

#conditionA and conditionB - both conditionA and conditionB should get satisfied
#conditionA or conditionB - any of conditionA or conditionB should get satisfied

#and example
#a=10
#b=20
#c=30
#d=40

#if a<b and a<c and a<d:
#print("hello")

#or example
#a=10
#b=20
#c=30
#d=40

#if a<b or a>c or a<d:
#print("hello")

#--------------------------------------------

#Types of operators-
#1) Arithmetic: + - * /
#2) Relational: == > < >= <= !=
#3) Logical: and or

#--------------------------------------------

# Here first a/2 gets calculated and then this value is assigned to b 
# Not that b is assigned a and then divided by 2

# a=10
# b=a/2 #assignment happens from right to left
# print(b)

#--------------------------------------------

# for loop in python

# you can use any variable but we generally use i
# range is used to define range of numbers
# range(a,b) - from a to b-1

# for i in range(1,11):
#     print(i)
    
# Multipliaction Table of 5
# 5*1
# 5*2
# 5*3
# .
# .
# .
# Here 5 is constant only 1,2,3,4 changes
# So we did 5*s and we put a range on s from 1 to 10

# for s in range(1,11):
#     print("5*",s,"=",5*s)

#--------------------------------------------

# Imp note reagrding print-

# " " prints things inside it as it is, no computation is done
# dont use " " if you want to do computation in print statememt
# to seperate computation and " " part we use commas

#--------------------------------------------

# While loop in python

# Structure of basic while loop
# i=0 #Initial value
# while i<101: #Condition
#     print(i) #Your code which you want to put in loop
#     i+=2 #Incrementor

#--------------------------------------------

# Boolean in python - True/False (Imp - T and F should be in capital)

# a="True" # This is string
# a=True # This is boolean
# print(type(a))

#--------------------------------------------

#Break statments in python

# b = 0
# while b<100:
#     print(b)
#     if b==5:
#         print("5 aa gaya")
#         break
#     b+=1

# break in used to break a loop if some condition gets satisfied.

# while loop to for loop conversion

# for i in range(0,100):
#     print(i)
#     if i==5:
#         print("5 aa gaya")
#         break

#--------------------------------------------

# Steps in for Loop (by default step is 1)

# for i in range(0,100,5):
    # print(i)

#--------------------------------------------

# Multiline comments

# This is single line comment

'''
This is multiline comment
'''

"""
This is also multiline comment
"""

'''
FOR EXAMPLE-
 a b c d e f g h i j k
 l m n o p q r s t
 u v w x y z
'''

#--------------------------------------------

# Multivariable assignment in one line
# Imp- number of variables should be = number of constants
# 1st variable maps to 1st constant, 2nd variable maps to 2nd constant and so on...

# a,b,c=10,10,10
# print(a,b,c)

#--------------------------------------------

# Lists in python

# Defining list
# mylist = [1,8,9,0,5,8,9]
# print(mylist)

# Indexing in list - starts from 0 till (length of list-1)
# mylist = ["firecracker","candy","suv","dfghj","svy"] # We saw on complex strings
# mylist = [True, False] # We saw on boolean
# mylist = ['a','b','c','d'] # We saw on simple strings
# print(type(mylist))
# print(mylist[2])

# List can store multiple datatypes at once
# mylist = ["Sarthak", 2, 5.6, "Sanidhya"]

# Length of list
# print(len(mylist))

# Maximum element in list
# print(max(mylist))

# Sorting in list

# IMP - mylist.sort() is a task and hence you cannot print the task
# You can print the list once mylist.sort() completes its task
# mylist.sort() #for ascending
# mylist.sort(reverse=True) #for descending
# print(mylist)

# Question - print all elements of list.
# The code should print all elements even we add/remove elements from list

# Method 1
# a=len(mylist)
# for i in range(0,a):
#     print(mylist[i])

# Method 2 (Smarter!)
# for element in mylist:
#     print(element)

# Taking input for a list from a user
# mylist=input("give a list of objects: ")
# print (mylist)

#--------------------------------------------

# String operations in python (same concept as list)

# name="sarthak"
# print(name[3]) #for indexing
# print(len(name)) #for length

#--------------------------------------------

# Update/Add elements in list

# Updating element in a list
# list = ["candy", "firecracker", "suv"]
# print(list[1])
# list[1] = "bike"
# print(list[1])
# print(list)

# Adding new elements to a list
# list = ["candy", "firecracker", "suv"]
# list.append("laptop")
# list.append("phone")
# list.append("bottle")
# print(list)
# Note that the elements are getting added at the end of the list

#--------------------------------------------

# Delete element from a list

# list = ["candy", "firecracker", "suv", "flower", "baloons"]
# print(list)
# del list[2]
# print(list)

# del list[i] deletes element at index i

#--------------------------------------------

# IMPORTANT OBSERVATIONS -

# Single element print karna hai toh indexing use karo

# All elements one by one print karna hai toh
# Method1 - for loop me lagake indexing karke print karo
# Method2 -
# for element in list:
#     print (element)

# Immutable are the constants which stay the same - eg: int, float, str, bool
# Mutable are the which can be changed - eg: lists, dictionary

#--------------------------------------------

# Question - Take a list and and then an element from user. Add this element to the list

# input_list = eval(input("Enter a list: "))
# print(input_list)
# new_element = eval(input("Enter the new element you want to enter in the list: "))
# input_list.append(new_element)
# print(input_list)

#--------------------------------------------

# Dictionary in python

# dict = {"sarthak": 98, "ayansh": 67, "rajesh": 89}
# print(dict)

# each thing is called an entry ("sarthak": 98)
# here "sarthak" is key and 98 is the value of that key
# keys must be unique - keys kabhi bhi repeat nahi honi chahiye

# The key and the value can be of any data type -
# dict1 = {"sarthak": "ayansh", "rajesh": "sumit"}
# dict2 = {98: 5, 100: 1, 45: 87}
# print(dict2)
# print(dict1)

# In list, the order in which you write the elements is always maintained
# In dictionary, this order is not maintained
# In dictionary, the relationship of key and value pair is maintained

# Question - Tell priority of the gifts you want using dictionaries only
# dict3 = {1: "firecracker", 3: "suv", 2: "candy"}
# print(dict3)
 
 #--------------------------------------------

# Accessing element using its key
# print(dict["sarthak"])

# Note the following:
# In list - we access element using its index beacuse in list elements are stored according to their indexes
# In dictionary - we access element using its key beacuse in dictionary elements are stored according to their keys

# This prints all keys of the dict one by one
# for i in dict:
#     print(i)

# This prints all values of the dict one by one
# for w in dict :
#     print(dict[w])   

# This prints all the keys and values one by one
# for i in dict:
#     print(i,"=",dict[i])

#--------------------------------------------

# Prints keys and values of respectively -
# print(dict.keys())
# print(dict.values())

#--------------------------------------------

# Updating/Adding element in dictionary -

# Updating element in dictionary
# dict["sarthak"]=99
# print(dict)
# dict["ayansh"]=98
# print(dict)

#--------------------------------------------

# IMP - String Concatenation

#String1 + String2 = A big string which is just concatenation of both the strings
# Eg - "Sar" + "thak" = "Sarthak"

# Note - both parts should be a string so that concatenation can be done
# "Sar" + 1 has no meaning bcs "Sar" is string and 1 is int
# But "Sar" + str(1) is valid bcs now 1 is also string since we applid str() function on it

#--------------------------------------------

# Deleting element from dictionary -

# Deleting element from dictionary is same as deleting element from list, the only difference is that in list, we use index numbers and in dictionary we use keys
# dict={"dog":4, "cat":4, "pigeon":2, "spider":8}
# print(dict)
# del dict["pigeon"]
# print(dict)

#--------------------------------------------

# Accessing keys and values in a dictionary -

# 1. If key in dict -

# if "pigeon" in dict:
#     print("present")

# if "pigeon" in dict: -> is used to check if the key pigeon is present in the dictionary

# 2. If key in dict.keys():

# print(dict.keys())
# This will give you a list of keys of our dictionary

# if "pigeon" in dict.keys():
#     print("present")

# 3. If value in dict.values()
    
# print(dict.values())
# This will give you a list of values of our dictionary

# if 4 in dict.values():
#     print("present")

#--------------------------------------------

# Printing lengths of lists and dictionaries

# list=[11,22,33,44,55,66,77,88,99,100]
# print(len (list))

# dictionary={"apple":"red","mango":"yellow","spinach":"green"}
# print(len(dictionary))

#--------------------------------------------

# Nesting -
# Nesting means ek ke andar ek

# Dictionary in List -

# Structure - [ {Dictionary1}, {Dictionary2}, {Dictionary3} , .... ]
# dictinlist = [ {"dog":4, "cat":4, "piegon":2} , {"Sarthak":100, "Sanidhya": 50} , {"bike":"KTM","car":"ferrari"}]
# print(dictinlist)

# Kitne baar bhi nesting kar sakte hai -
# dictinlistinlist=[[{"dog":4, "cat":4, "piegon":2} , {"Sarthak":100, "Sanidhya": 50} , {"bike":"KTM","car":"ferrari"}],[{"dog":4, "cat":4, "piegon":2} , {"Sarthak":100, "Sanidhya": 50} , {"bike":"KTM","car":"ferrari"}]]
# print (dictinlistinlist)

# Similarly-

# List in Dictionary -

# Here only the values can be list, keys cannot be list
# Keys cannot be a mutable data type beacuse mutable datatypes can be changes and keys are made to be unique and hence unchangable
# listindictionary={"sarthak's list":["dog","cat","rabbit","bird"]}
# print(listindictionary)

# List in List -

# list=[[1,2,3,4,5,6,7,8,9,10],[11,12,13,14,15,16,17,18,19,20],[21,22,23,24,25,26,27,28,29,30]]
# This is called as 2d list
# [ [ [], [], [] ], [ [], [] , [] ], [ [], [], [] ] ] - This is list in list in list - 3 levels of list nesting hence it is called as 3d list
# print(list)

# Dictionary in Dictionary - 

# Here also the keys cannot be mutable datatyepes (lists or dicts) hence only str, int, float, bool
# dict={1234589:{"blah":5}}
# dict1={True:{"blah":5}} # Can be boolean as well
# print(dict)
# print(dict1)

# # We can store any datatype in a list-
# list = ["sarthak", 8, 9.5]
# print(list)

# list1 = [ [1,2,3], "sarthak", {"dog": 2, "pigeon": 2}, 8, 9.5, False]
# print(list1)
# # Here we stored a list, str, dict, int, float, bool - everything together

# # We can store any datatype in a dictionary as well-
# dict = {"sarthak": 12, "sanidhya": [1,2,3], "rahul": 2.2, "mukesh": True, "shobhit": "hello", "suresh": {"dog": 4, "cat": 4, "pigeon": 2}}
# print(dict)
# Here we stored a int, list, float, bool, str, dict - everything together
# Only important point is that the key cannot be list or dict, it can only be str, int, float, bool

#--------------------------------------------

# LIST OPERATIONS -

# Comparing two lists-

# list1=[1,5,24,25]
# list2=[1,7,4,6]
# we will compare these lists by cheking their first indexes in both the list, if they are equal we will move on and do the same
# So here list1 < list2

# list1=["sarthak", "arnav"]
# list2=["sarui", "ronak"]
# print(list1<list2) # This will print True

# List calculations-

# list1=[1,5,24,25]
# list2=[1,7,4,6]
# list3 = list1 + list2 # This will print list 2 after list 1. This is known as joining two lists.
# list4 = list1 * 5 # This will print list 1 five times.
# print(list4)

# List slicing-

# listy=[1,5,24,25,7,4,6,45,26,8]
# I want the output to be as a list which has elements of listy from 24 to 4
# index of 24 - 2
# index of 4 - 5
# print(listy[2:6])

# listy[ starting index : ending index + 1] -  format for slicing
# print(listy[1:9])

# print(listy[1:9:2]) # This prints elements from index 1 to 8 with a step size of 2
# listy[ starting index : ending index + 1 : stepsize] -  format for slicing with stepsize
# By default step size is 1

# List reversal-
# t=[1,2,3,4,5,6,7,8,9]
# t.reverse()
# print(t)
# print(t.reverse()) will give you an error since it is a task and printing task has no meaning.

#--------------------------------------------

# String Handling -

# Checking for strings in strings -

# a = "Ahmedabad"

# # This code will give answer as present
# if "bad" in a:
#     print("present")
# else:
#     print("absent")

# # This code will give answer as absent
# if "Bad" in a:
#     print("present")
# else:
#     print("absent")

# "bad" and "Bad" are very different

#--------------------------------------------

# String Comparisons -

# small a is greater than capital a
# "a">"A" ->  True
# "abcD" > "abcd" -> False
# Check all elements one by one and decide, if same then move forward and repeat

#--------------------------------------------

# String slicing -

# Everything is same as in list
# a = "amazing"
# print(a[3])
# print(a[0:4:2]) 
# print(a[3:])
# print(a[:5:2]) # All elements from start till index 4 with step size of 2
# print(a[:])
# print(len(a))
# print(a[:90]) # This will print all characters of strings till the index where characters are present
# print(a[90]) # This will give an error since no element at index 90

# a[3] - These kinds of commands are used to access a single element in a string
# a[3:7] - These kinds of commands are used to access a slice of the string 

# Negative indexing-

# a  m  a  z  i  n  g
# -7 -6 -5 -4 -3 -2 -1

# print(a[-1]) # This will print g

# negetive indexing is when you use negetive integers to perform this task
# it works using negetive integers like -1,-2,-3,-4,-5,-6
# the last number is not counted like n positive indexing

# print(a[-7:-3]) # This will print -7 to -4 which is "amaz"
# print(a[-3:-7]) # This will print nothing as we are asking it to go in opposite direction

# IMPORTANT POINTS -

# Combination of positive and negative indexing
# a[3:-5] -> start from index till index -5-1 which is till -6

# Concatenation-
#a[:3] -> ama
#a[3:] -> zing
#a[:3] + a[3:] -> amazing (concatenation of above two things)

#a[:-7] -> Empty
#a[-7:] -> amazing
#a[:-7] + a[-7:] -> amazing (concatenation of above two things)

#Negative indexing examples-
#a[1:6:2] -> start from index 1 till 5 with step size of 2
#a[-7:-3:3] -> start from index -7 till index -4 with step size of 3
#a[-7:-3:-3] -> start from index -7 till index -4 with step size of 3 but in reverse order
#a[:] -> print all elements
#a[::2] -> print all with step size of 2
#a[::-2] -> print all with step size of 2 in reverse order
#a[::-1] -> print all in reverse order

# To reverse a word -
# word = "Sarthak"
# print(word[::-1])

#--------------------------------------------

# Small Note -

# a = 3
# print(a)

# # We will just update the value of a. no new variables would be created.

# # a = a+1 # first do a+1 then assign this we calculated value to a
# # print(a)
# # a+=1 #this is same as a=a+1
# # print(a)

# a = a-2
# a-=2 # same as a = a-2

# a = a*2
# a*=2

# a = a/2
# a/=2

#--------------------------------------------

# FUNCTIONS IN PYTHON -

#Defining a function named dumb which takes 3 numbers - num1, num2, num3 as input, adds all of them, and returns num3 as output
# def dumb(num1, num2, num3):
#     final_answer = num1+num2+num3
    # return final_answer

#Calling the function named dumb
# output = dumb(1,2,3)
# print(output)

#Structure for writing a function-
# def <function name> (<all input things>):
#     #operation which you wish to perform
#     return <output>

#Example to understand better

# def idiot(num1,num2,num3):
#     final_answer = num1*num2+num3
#     return final_answer

# a=int(input("enter num 1: "))
# b=int(input("enter num 2: "))
# c=int(input("enter num 3: "))

# no_brain=idiot(a,b,c)
# print(no_brain)

#--------------------------------------------

# Void Functions - these functions do not return anything. 
# They just take the input and process/print etc. Nothing is returned.

# num=1

# def printer(num):
#     if num==1:
#         print ("num  = 1")
#     if num==2:
#         print ("num = 2")

# printer(num) # This function call justs calls the function, nothing is returned by the function

#--------------------------------------------

# Variable input in a function - (*)

# In the below code, number of inputs are fixed to 4. So, if I want to pass 5 inputs to this function, then it'll show error
# def grind(num1, num2, num3, num4):
#     maxnum = max(num1, num2, num3, num4)
#     return maxnum

# final_output = grind(1,2,3,4,5) # Show error because you are passing 5 things as input and the function is accepting only 4. hence, error occurs
# print(final_output)

# To handle the above problem, we used *nums. *nums takes cares of variable number of inputs. You can pass any number of inputs now.

# def grind2(*nums):
#     maxnum = max(*nums)
#     return maxnum

# final_output2 = grind2(1,2,3,4,5,6) # Now you can enter 5 inputs, 4 inputs, etc. Any number of inputs as per your wish.
# print(final_output2)

#--------------------------------------------

# Here, *nums is a kind of list.
# To access, any element you may use indexing just like you did in lists

# def myfunc(*nums):
#     answer = nums[2]
#     return answer

# final_output = myfunc(1,2,3,4,5,6)
# print(final_output)

# Question - Take few numbers and an index as input for a function and return the number at the given index

# def myfunc(*nums, index):
#     answer = nums[index]
#     return answer

# # final_output = myfunc(76, 34, 54, 67, 32, 3)
# # Above line will cause an issue because you want to pass 3 as index here.
# # But the computer will not understand that and it will consider 3 as a part of *nums only.

# # So, to resolve this, we specially mention index=3 while calling the function as follows-
# final_output = myfunc(76, 34, 54, 67, 32, index=2)

# print(final_output)

#--------------------------------------------

# Important -

# if there is "return" statment in a function then only that function will return you something
# Otherwise, nothing is returned

# if function return something then you need to worry about capturing the output
# Otherwise, no need to worry.

# A function will only work when it is called otherwise it will not!

# you can return multiple things from a function, make sure to capture everything
# def func(a):
#     out1 = 0
#     out2 = 1
#     out3 = 2
#     return out1, out2, out3

# #Input -> a
# #Output -> out1, out2, out3

# output1, output2 = func(3) #This will give you error bcs function returns 3 things and you are capturing only 2 things 
# output1, output2, output3 = func(3)
# print(output1)
# print(output2)
# print(output3)

#--------------------------------------------

# Various kinds of function-

'''
def myfunc1(a):
    return a+1
Input -> a
return -> a+1

output = myfunc1(3)
print(output) # This will print 4

def myfunc2(a):
    a = a+1
Input -> a
return -> nothing

output = myfunc2(3)
print(output) # This will print nothing beacuse func is not returning anything

def myfunc3():
    return 4
Input -> nothing
return -> 4 

output = myfunc3()
print(output) # This will print 4
'''

#--------------------------------------------

#Local variables and Global variables

# sum = 1 # This sum variable is a global variable.
# # Global variable -> These are those variables which are known to everyone in the program.
# # These are written outside any code block

# def myfunc():
#     newsum = 0 # This newsum variable is a local variable.
#     # Local variable -> These are those which are known only to the code block where this local variable is present
#     print(sum) # Prints 1 beacuse sum is a global variable and hence myfunc knows the value of sum
#     print(newsum) # Here it will print 0 bcs newsum is local variable for this code block

# myfunc()
# print(newsum) # This will give error beacuse newsum is known only by the code block where it is present

#--------------------------------------------

# Recursion Functions -> VIMP

# def call3():
#     return "hello i am call 3"

# def call2():
#     outputfrom3 = call3()
#     return outputfrom3

# def call1():
#     outputfrom2 = call2()
#     return outputfrom2
    
# outputfrom1 = call1()
# print(outputfrom1)

# Classic Recursion Function (factorial)-

# Recursion - a function that calls itself until the base case comes.

# def factorial(n):
#    if n == 1:
#        return 1 # This is called as base case. This does not produce a function call further. Helps in ending recursion.
#    else:
#        return n * factorial(n-1) # This is where recursion happens. This leads to another function call

# num = eval(input("enter a number"))
# output = factorial(num) # Initial function call
# print("The factorial of", num, "is", output)

# Always make a recursion tree while solving recursion questions
# Recursion is done by using FUNCTIONS. -> IMP

# Solved Questions on Notebook (Factorial and Fibonacci Series questions - recursion tree)

#--------------------------------------------

# Matplotlib in Python

# import matplotlib.pyplot as plt -> Used for plotting things (line graph, bar graph, pie chart, etc)
# Can import as pgd also, anything, but generally we use plt

# Studied what are ipynb? Studied cells?
# ipynb -> i python notebook
# cells -> blocks of code in ipynb

# Worked on Colab - https://colab.research.google.com/drive/1mhKy5poaOkSot_HI-3Tu27Qgqqn6kmyJ#scrollTo=eY7li63fwDW-

# Further Reading - https://www.w3schools.com/python/matplotlib_intro.asp 

#--------------------------------------------

# HackerRank Questions Important Points
# Link - https://www.hackerrank.com/domains/python 

# split() - 
# It will break the input into different parts, each being a element of a list
# a = input("Enter your name: ").split()
# print(type(a)) #type is a list
# print(a)

#strip()-


# Exponents in python
# " ** " is used to get exponents
# print(3**2)  # -> This will print 9
# Other way of doing this is: print(3*3)

# End -> it is used to define how the print statment will end.
# by default end="/n"
# end can only be used in a print statement
# eg:  
# for i in range (0,10):
#     print(i, end="H")
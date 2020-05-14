# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

# Demo file for Spyder Tutorial
# Hans Fangohr, University of Southampton, UK
import csv

def hello():
    """Print "Hello World" and return None."""
    print("Hello World")

# Main program starts here
hello()
with open('data.csv', newline='') as csvfile:
   spamreader = csv.reader(csvfile, delimiter=',')
   for row in spamreader:
       print('|'.join(row))
import csv
import sys

dim = int(sys.argv[1])

arr = [[(r//2)+(c//2)*100000 for r in range(dim)] for c in range(dim)]

with open("layouts/"+str(dim)+"x"+str(dim)+".csv","w+") as my_csv:
    csvWriter = csv.writer(my_csv,delimiter=",")
    csvWriter.writerows(arr)

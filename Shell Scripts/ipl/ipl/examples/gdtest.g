Proc Getdata:
Data: 
1 2 3 4 5 
1 2 3 4 5 
1 2 3 4 5 
1 2 3 4 5 
1 2 3 4 5 
Selectfields: 1 3 5

Proc Getdata:
Join: y
Data: 10 11
	10 11
	10 11
	10 11
	10 11

Proc Getdata:
Append: y
Data: z z z z z
      z z z z z
      z z z z z

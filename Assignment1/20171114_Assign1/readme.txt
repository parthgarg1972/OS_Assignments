Question 1:
	Commands used:
		1 mkdir - to make the directory and give rwx------ permissions.
		2 stat - to check the exsitence of a directory.
		3 open - to open the file.
					Attributes - O_RDONLY, O_WRONLY, O_CREAT.
		4 lseek - to change the file pointer
					Attributes - SEEK_END, SEEK_SET.
		5 read - to read n bytes from input file.
		6 write - to write n bytes into output file.
		7 close - to close the file.

		In order to optimize the problem, I am taking blocks of 10 MB at a time.
		
		permissions of directory 700.
		permissions of file 600.


		Format:
			gcc OS_ques1.c
			./a.out <input_file.txt>


Question 2:
	Commands used:
		The commands used are same as first question.
		Symlink - to make symlink.
		symlink is created in the present directory.
		The program of first question is used in this question to verify the output file.	

		Format:
			gcc OS_ques2.c
			./a.out <input_file.txt>	

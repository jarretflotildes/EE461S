      //User wants to Pipe
      if(strstr(command,"|")){
	pid = fork(); //child takes care of piping children
	if(pid == 0){
           pid = fork();  	   
	   if(pid == 0){
	      close(pipefd[0]);              //close unused read end 
              dup2(pipefd[1],STDOUT_FILENO); //output goes to right end ok pipe toke
              execvp(tokens[0],tokens); 
	      exit(0);
	   }

	   pid = fork();
	   if(pid == 0){
	      close(pipefd[1]);              //close unused write end
              dup2(pipefd[0],STDIN_FILENO);
	      execvp(tokens[2],tokens);
	      exit(0);
	   }

	   close(pipefd[0]);
	   close(pipefd[1]);
           waitpid(-1,&status,0);
	   waitpid(-1,&status,0);
	   exit(0);
	} else{
           waitpid(pid,&status,0);       //parent waits for child in charge of piping children
	}
  

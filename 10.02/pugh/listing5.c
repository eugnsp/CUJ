   #include <stdio.h>
   #define SIZE_BUFFER 1024
   #include <sys/types.h>
   #include <sys/stat.h> 
#ifdef BSD
   #include <sys/file.h>  /* This header name varies */
#else 
   #include <fcntl.h>        
#endif
   static int copy_files();

   truncate_file(path, length)
   /* Truncates a file to the length specified */
   char *path;
   int length;
       {
       int input_file, temp_file;
       char temp_file_name[L_tmpnam];
       struct stat file_status;
       int ret;

       ret = 0;
       if (length < 0)
           {
           ret = -10;
           goto end;
           }

       /* If 0 length, then just open/close it */
       if (length == 0)
           {
           input_file = open(path, O_WRONLY | O_TRUNC, 0666);
           if (input_file < 0)
               {
               ret = -1;
               goto end;
               }
           close (input_file);
           goto end;
           }

       input_file = open(path, O_RDONLY, 0);
       if (input_file < 0)
           {
           ret = -1;
           goto end;
           }

       /* Check the file length - no sense copying if shorter 
already */
       fstat(input_file, &file_status);
       if (length > file_status.st_size)
           {
           close (input_file);
           ret = -3;
           goto end;
           }

       /* Open a temporary file to hold the contents to be copied 
back */
       tmpnam(temp_file_name);
       temp_file = open(temp_file_name, O_RDWR | O_CREAT, 0666);
       if (temp_file < 0)
           {
           ret = -2;
           goto end;
           }

       /* Make a copy of the file */
       ret = copy_files(input_file, temp_file, length);
       if (ret == 0)
           {
           lseek(temp_file,0L,0);
           close(input_file);
           input_file = open(path, O_WRONLY | O_TRUNC, 0666);
           if (input_file < 0)
               ret = -12;
           else
              { 
              ret = copy_files(temp_file, input_file, length);
              close(input_file);  
              } 
          }

     close (temp_file);  
     unlink(temp_file_name);    

end:
     return ret;
     }
   

static int copy_files(file_in, file_out, length)
/* Copies from file in to file out */
int file_in;
int file_out;
int length;
       {
       int ret;
       char buffer[SIZE_BUFFER];
       int read_length;
       int write_length;
       int length_to_read;
       int length_to_write;        
       ret = 0;    
       while (length > 0)
           {
           if (length > SIZE_BUFFER)
               length_to_read = SIZE_BUFFER;
           else
               length_to_read = length;
           length -= length_to_read;
           read_length = read(file_in, buffer, length_to_read);
           if (read_length != length_to_read)
               {
               ret = -4;
               goto end;
               }
           length_to_write = read_length;
           write_length = write(file_out, buffer, 
length_to_write);
           if (write_length != length_to_write)
              {
              ret = -5;
              goto end;
              }                    
          }        
end:
    return ret;
    }    

main()
    {
    printf("\n Ret is %d", truncate_file("a:temp", 1));
    }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 10

char *tests[] = {"ADD x5, x13, #15\n","SUBS x29, x15, #440\n","ADDS x0, x29, #33\n",
                 "SUB x10, x20, #20\n","MOV x0, #3014\n","MOVN x10, #123\n",
                 "MOVK x4, #9921\n","\tADD x3, x4, #12\n","\tSUBS x4, x5,   #33   \n",
                 "MOVN x29, \t#1123\n"};
char *answers[] = {"0x91003da5\n","0xf106e1fd\n","0xb10087a0\n","0xd100528a\n",
                   "0xd28178c0\n","0x92800f6a\n","0xf284d824\n","0x91003083\n",
                   "0xf10084a4\n","0x92808c7d\n"};

char* ignore_lines(char *buffer, int lines){
   if(lines < 0){
      return NULL;
   }
   while(1){
      char c = *(buffer++);
      if(c == '\n'){
         lines--;
      }
      if(lines <= 0){
         break;
      }
   }
   return buffer;
}

void writeTest(){
   FILE *f = fopen("grading_r.txt","w");
   int count;
   for(count = 0; count < NUM_TESTS; count++){
      fprintf(f,"%s",tests[count]);
   }
   fclose(f);
}

char* get_line(char *buffer, char **line){
   char c;
   int i = 0;
   while(1){
      c = *(buffer++);
      if(c == '\n' || c == 0){
         line[0][i++] = '\n';
         line[0][i] = 0;
         return buffer;
      }
      line[0][i++] = c;
   }
   return buffer;
}

int main(){
   char *buffer = (char*)malloc(4000*sizeof(char));
   writeTest();
   system("gcc -o ensamblador ensamblador.s");
   FILE *f = popen("./ensamblador grading_r.txt","r");
   int i = 0;
   while(1){
      char c = fgetc(f);
      if(feof(f)){
         break;
      }
      buffer[i++]=c;
   }
   char *aux_buffer = ignore_lines(buffer,15);
   char *line = (char*)malloc(100*sizeof(char));

   i = 0;
   float total = 0.0;
   for(i = 0; i < NUM_TESTS;i++){
      aux_buffer = get_line(aux_buffer,&line);
      printf(".............................\nInst: %sObtenido: %sEsperado: %s",tests[i],line,answers[i]);
      if(!strcmp(line,answers[i])){
	  total+=100.0/NUM_TESTS;
          printf(" (+%.2f)\n",100.0/NUM_TESTS);
      }else{
         puts(" (+0.00)");
      }
   }
   printf(".............................\nTotal: %.2f/100.0\n",total);
   free(line);
   free(buffer);
   return 0;
}

#include <stdio.h>        // for printf()
struct Person {
  char   *name;
  int     age;
  char    gender; // use 'm', 'f'
  double  shoeSize;
};
void populatePerson (struct Person *person)
{
  person->name = "Bork";
  person->age = 34;
  person->gender = 'm';
  person->shoeSize = 10.5;
} // populatePerson
/*void printPerson (struct Person *person)
{
  printf ("name:
printf ("age:
	  printf ("gender:
%s\n", person->name);
	  %d\n", person->age);
%s\n",
	  (person->gender == 'm') ? "male" : "female");
  printf ("shoe size: %f\n", person->shoeSize);
  } // printPerson*/
int main (int argc, char *argv[])
{
  struct Person me;
  populatePerson (&me);
  // printPerson (&me);
  return (0);
} // main

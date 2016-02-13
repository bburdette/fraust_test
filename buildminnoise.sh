
g++ -c minimal.cpp -o minimal.o
ar rcs libminimal.a minimal.o
g++ -static minmain.cpp -L. -lminimal -o minnoise
#  ./statically_linked
 

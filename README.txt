DCC888 - Second Project - Dead Code Elimination                                  
Students: Angélica Moreira (2018718236) and Roberto Rosmaninho (2018054940)        

------------------                                                              
Compiling the pass                                                              
------------------                                                              

The Makefile for this pass should be generated using CMake. We provide the 
CMakeLists.txt file within.

To generate a Makefile, execute the build.sh script which is inside the folder
 that contains the source code of this
first project, which is TP2/deadcodeelimination, indicating the root path where
 your LLVM is built, as follows:

./build.sh [path/to/llvm] # you should run this command inside the folder 
deadcodeelimination     

This script will execute the following command: cmake -DBUILD_SHARED_LIBS=ON 
$LLVM_BUILD_DIR ..

After that the Makefile should be generated and the shared library can be 
built by simply running a "make" command, which is being executed in our 
build.sh script as follows: make -j8                     

After running the build.sh it  will also generate a build folder and inside a 
shared library called [libDeadCodeAnalysis], which is our compiled pass.

Pay attention that this library may have either the extension .dylib (if your 
OS is MacOS) or .so (if your OS is Linux).

--------------------------------                                                
Running the pass on a given file                                                
--------------------------------                                                

To run our pass on a .cpp or .c file, execute the execute.sh script (which is 
inside the TP1/CfgGenerator/src folder), passing the path to the file, as follows:

./execute.sh [path/to/llvm] [path/to/file.cpp or path/to/file.c]                               

The execute.sh script will execute the following commands:                       

clang or clang++ followed by -Xclang -disable-O0-optnone -S -emit-llvm 
-o $program_ll $program_file  

opt -mem2reg -instnamer -mem2reg -break-crit-edges $program_ll -S -o $program_ll

opt -load $dead_code_analysis -vssa -dead-code-ra -simplifycfg -stats -S 
-o=$program_ll < $program_ll

opt -dot-cfg $program_ll -disable-output

After that, you should be able to see the dot files generated inside the folder 
dot_files in the same folder of the execute.sh script.
Pay attention that a folder with the name of the program being analysed will be 
created inside the folder dot_files and all the dot files
generated for this program will be inside this folder. Those files can be 
visualized if you have installed the graphviz tool but if you
do not you can convert it by executing the following command: 
dot -Tpdf [file_name].dot -o [file_name].pdf.

Obs.: The example programs of the second project description is inside the 
folders TP2/deadcodeelimination/TestFiles/Test1, 
TP2/deadcodeelimination/TestFiles/Test2 and 
TP2/deadcodeelimination/TestFiles/Test3; I also provide some other programs, 
that was used to test this implementation.                 

Obs2.: The source code of our pass is inside the folder 
TP2/deadcodeelimination/lib/DeadCodeAnalysis
and it was registered with the name: -dead-code-ra.

Obs3.: I put the Range Analysis source code together with my pass, thus I build 
only one library.

Obs.: If you want to see the debugging information you can add the flag 
-debug-only=DeadCodeAnalysis in the execute.sh file as follows:

opt -load $dead_code_analysis -vssa -dead-code-ra -debug-only=DeadCodeAnalysis 
-simplifycfg -stats -S -o=$program_ll < $program_ll

--------------------------------                                                
Description of the behaviour                                                
--------------------------------  

Our pass marks dead blocks as unreachable by by adding store instructions with 
undefined values doing the same what it is done in the pass InstCombine 
(more details in InstCombineInternal.h). We also remove some of those basic blocks
by invoking removeUnreachableBlocks(function) from internal library of llvm
(llvm/Transforms/Utils/Local.h, which is a class that has functions to perform 
local transformations). We also remove dead instructions from the basic blocks 
using the function RecursivelyDeleteTriviallyDeadInstructions(instruction) from 
llvm/Transforms/Utils/Local.h as advised in the description of this project. 

Although we do remove the majoriti of basic blocks that are we marked as unreachable,
some of them are not removed, only their dead instructions, and we end up with
basic blocks that contains only an indirect branch to another block. And those kind
of blocks are essentially redundants, thus we use the simplifycfg pass after ours 
to create a clean CFG for the output program. 

Thus our pass only give the statistics of what our pass marked as unreachable, and
the amount of instructions and basic blocks that it was possible to remove entirely
by itself. We do not account for the ones simplifycfg removed. Since you wrote in 
the description of this project: "Feel free to use as much help from LLVM as you want."
We decided to use as much help as we could.
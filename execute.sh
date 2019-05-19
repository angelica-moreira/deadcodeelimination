#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Usage: $0 [Path] [Program]";
  exit 1;
fi

dir=$1;
if [ ! -d "$dir" ]; then
  echo "Invalid path: ${dir}";
  exit 1;
fi

#set the lib sufix
suffix="dylib"
if [[ $(uname -s) == "Linux" ]]; then
  suffix="so"
fi

script_path="$( cd "$(dirname "$0")" ; pwd -P )"
program=${2%.*}
extension=${2#*.}
program_ll=${program}.ll
program_file=$2
program_file_name=$(basename -- "$2")
extension=${program_file_name#*.}
program_file_name=${program_file_name%.*}
pass_path="$(dirname "$script_path")"
dead_code_analysis=${script_path}/build/libDeadCodeAnalysis.$suffix

echo $dead_code_analysis

directory_name="$(dirname "$program")"
#clean all files from the current program files
function clean() {
  echo "Removing old files."
  rm -f *.bc
  rm -f *.ll
  rm -f *.o
  rm -f *.exe
  rm -f *.out
}

function compile(){
  echo "Compiling"

  if [ $extension == "cpp" ]
   then
   clang++ -Xclang -disable-O0-optnone -S -emit-llvm -o $program_ll $program_file
  else
   clang -Xclang -disable-O0-optnone -S -emit-llvm -o $program_ll $program_file
   fi
}

function load(){
  echo "====== Loading some optimizations ========"
  opt -mem2reg -instnamer -mem2reg -break-crit-edges $program_ll -S -o $program_ll

  echo "====== Loading Dead Code Elimination Analysis pass with some auxiliary passes ======="
  opt -load $dead_code_analysis -vssa -dead-code-ra -simplifycfg -stats -S -o=$program_ll < $program_ll

  echo "===== Generating the cfg.dot files ======="
  opt -dot-cfg $program_ll -disable-output
}

#move all dot files to the program_dot_files_generated                          
function moveFiles(){                                                           
  #It creates a directory that will be the repository for all dot files         
  directory_name_dot_files=${script_path}/dot_files                             
  if [ ! -d "$directory_name_dot_files" ]; then                                 
      mkdir -p $directory_name_dot_files                                        
  fi                                                                            
                                                                                
  #It creates a directory that will be the repository for the dot files of the current program
  directory_name_program_dot_files=${directory_name_dot_files}/${program_file_name}
  if [ ! -d "$directory_name_program_dot_files" ]; then                         
      mkdir -p $directory_name_program_dot_files                                
  fi                                                                    
                                                                                
  #In case the dot files of the current code exists move it to the correct directory
  echo "Moving the generated dot files to the folder ${directory_name_program_dot_files}"
  cp .*.dot $directory_name_program_dot_files                                    
  rm -f .*.dot                                                                   
}    

cd $directory_name
clean ;
cd $script_path
compile ;
load ;
moveFiles ; 

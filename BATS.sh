#!/bin/bash
#Script Name : BATS.sh
#
#Description : Tests the maze program's functionality
#               and possible sources of error
#
#Comman line options: None
#
#Input: None
#
#Output: Make and Tests of the query program.
#
#Special considerations:
#
#Pseudocode:
#
#clean the previous logfiles(uncomment if needed)
#    make cleanLog
#
#Create log with todays time included
#Print the hostname and OS where the build was run
#
#make clean : cleanup unnecessary files
#
#make : make the file
#
#Test case with no options, 1 option, 2 option
#
#Test cases with all three options:
#Test case with -n invalid arguments
#    - Non numeral
#    - Out of bounds value
#    - Value missing
#    
#Test case with -d invalid arguments
#    - Non numeral
#    - Out of bounds value
#    - Value Missing
#
#Test case with -h invalid arguments
#    - invalid host (not pierce.cs.dartmouth.edu
#    - Value Missing
#
#Missing value for options
#
#
#make clean : last cleanup
#
#Print out the time the process ended
#
#*******************************************************************************************************
#CAUTION :                                          
#*******************************************************************************************************
#
#------------------------------------------------------------------
#

#Defines the hostname that we are going to use.
HostName="pierce.cs.dartmouth.edu"

#Clean all the previous logs
make cleanLog > /dev/null

#Create the logfile to save the actions
Now=$(date +"%a_%b_%d_%T_%Y")
FileName="./testing/MazeTestLog.$Now"
touch $FileName

#print the starting time
printf "Started at : $Now\n" >> $FileName
#Print the hostname and operating system
printf "Hostname : $HOSTNAME\nOS:" >> $FileName
uname >> $FileName

#Clean the unnecessary stuff
printf "\n________________________________________________________________\n" >> $FileName 
printf "Make Clean and delete all the unnecessary materials\n" >> $FileName 
printf ">make clean\n" >> $FileName 
make clean >> $FileName 

#Makes the maze program
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nUse Makefile to make maze.\n" >> $FileName 
printf ">Make maze \n" >> $FileName
make maze >> $FileName

#No options error
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nNo argument is input. Should tell which parameters to input.\n" >> $FileName 
printf ">maze\nOutput:" >> $FileName 
maze  2>> $FileName 

#Few options error
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nToo few argument input. Should tell which parameters to input.\n" >> $FileName 
printf ">maze -n 4 \nOutput:" >> $FileName 
maze -n 4 2>> $FileName 

#Too many arguments
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nToo many arguments are input. Should tell which parameters to input.\n" >> $FileName 
printf ">maze -n 4 -d 7 -h $HostName extra \nOutput:" >> $FileName 
maze -n 4 -d 7 -h $HostName extra 2>> $FileName 

#Combination of same options
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nCombination of same arguments. Should tell which parameters to input.\n" >> $FileName 
printf ">maze -n 4 -n 3 -d 7 \nOutput:" >> $FileName 
maze -n 4 -n 3 -d 7 2>> $FileName 

#Set of instructions
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nFrom now on, we test error cases with input values.\n" >> $FileName

#Test case with -n invalid argument (non-numerical)
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nNon numeral input for -n. Should tell user to input numerical values\n" >> $FileName
printf ">maze -n 4a -d 2 -h $HostName \nOutput:" >> $FileName 
maze -n 4a -d 2 -h $HostName 2>> $FileName 

#Test case with -n invalid argument (out of bounds)
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nOut of bounds input for -n. Should tell user to input within certain range\n" >> $FileName
printf ">maze -n 4a -d 2 -h $HostName \nOutput:" >> $FileName 
maze -n 15 -d 2 -h $HostName 2>> $FileName 


#Test case with -n invalid argument (argument missing)
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nMissing value for -n option. Should tell user to provide input value\n" >> $FileName
printf ">maze -d 2 -h $HostName ran -n \nOutput:" >> $FileName 
maze -d 2 -h $HostName ran -n 2>> $FileName 


#Test case with -d invalid argument (non-numerical)
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nNon numeral input for -d. Should tell user to input numerical values\n" >> $FileName
printf ">maze -n 4 -d 2b -h $HostName \nOutput:" >> $FileName 
maze -n 4 -d 2b -h $HostName 2>> $FileName 

#Test case with -d invalid argument (out of bounds)
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nOut of bounds input for -n. Should tell user to input within certain range\n" >> $FileName
printf ">maze -n 4 -d 15 -h $HostName \nOutput:" >> $FileName 
maze -n 4 -d 15 -h $HostName 2>> $FileName 


#Test case with -d invalid argument (argument missing)
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nMissing value for -d option. Should tell user to provide input value\n" >> $FileName
printf ">maze -n 4 -h $HostName ran -d \nOutput:" >> $FileName 
maze -n 4 -h $HostName ran -d 2>> $FileName 


#Test case with -h invalid arugment
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nHostname not same as predefined hostname : $HostName\n" >> $FileName
printf ">maze -n 4 -d 2 -h some.random.hostname \nOutput:" >> $FileName 
maze -n 4 -d 2 -h some.random.hostname 2>> $FileName 


#Test case with -h invalid argument (argument missing)
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nMissing value for -h option. Should tell user to provide input value\n" >> $FileName
printf ">maze -n 4 -d 2 ran -h \nOutput:" >> $FileName 
maze -n 4 -d 2 ran -h 2>> $FileName 

#Successful end
printf "\n________________________________________________________________\n" >> $FileName 
printf "\nSuccessfully completed testing AMStartup! Now, cleaning up the unnecessary files.\n" >> $FileName

make clean >> $FileName

#print the ending time
Now=$(date +"%a_%b_%d_%T_%Y")
printf "Ended at : $Now" >> $FileName


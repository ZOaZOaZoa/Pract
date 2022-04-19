********************************************************************
**                                                                **
**               IBM-PC Installation of DISLIN                    **
**                                                                **
**  Contents:  1.)  Introduction                                  **
**             2.)  Installation of DISLIN                        **
**             3.)  Getting DISLIN Support                        **
**                                                                **
**  Date   :   15.03.2021                                         **
**  Version:   11.4 / Windows 64-bit, gcc/g++/gfortran - Mingw64  **
**  Author :   Helmut Michels                                     **
**             Dislin Software                                    **
**             Am Hachweg 10                                      **
**             37083 Göttingen, Germany                           **
**  Email  :   michels@dislin.de                                  **
********************************************************************

1.)  Introduction

     This file describes the  installation  of the  data  plotting
     software DISLIN on IBM-PCs   for the operating system Windows
     and the Mingw 64-bit compilers GCC, G++ and GFortran.

     The DISLIN libraries are compiled with gcc version 4.5.2, which
     is available from the site http://tdm-gcc.tdragon.net.

2.)  Installation of DISLIN

     The DISLIN version for the Mingw64 compilers GCC and GFortran
     is distributed in the  zipped file  dl_11_mg.zip.  A utility
     for unpacking the  DISLIN  files is available  from the  same
     location where you have downloaded  dl_11_mg.zip, or from the
     DISLIN CD-ROM. 

     To install DISLIN, make the following steps:

  a) Create a temporary directory and copy the files dl_11_mg.zip
     and unzip.exe to it:

     for example:   md  c:\temp
                    cd  c:\temp
                    copy e:\dislin\windows\unzip.exe    *.*
                    copy e:\dislin\windows\dl_11_mg.zip *.*

  b) Extract the DISLIN files with the command:

                 unzip  -o dl_11_mg.zip

  c) Run the setup program with the command

                  setup

     -  Choose OK

     -  Give the Installation Directory where  DISLIN  should be in-
        stalled. The default directory is C:\DISLIN.

  d) Reconfigure the System

     Set the DISLIN environment variable to c:\dislin and include
     c:\dislin\win in your path. If you have installed DISLIN in a
     different directory, you have to use that directory for the 
     environment variables above.

     The environment variables can be set or modified with the Control
     Panel 
    
    (see Control Panel -> System -> Advanced -> Environment
     Variables).
   
  e) Now you can compile,  link and run the example programs in the
     DISLIN subdirectory examples with the commands

                    clink   -a  exa_c      (gcc) 
                    cpplink -a  exa_cpp    (g++) 
                    dlink   -a  exa_f77    (gfortran Fortran 77)
                    f90link -a  exa_f90    (gfortran Fortran 90/95)
     and
                    clink   -a  map_c      
                    cpplink -a  map_cpp      
                    dlink   -a  map_f77    
                    f90link -a  map_f90 

     Notes:
 
        -  clink, dlink and f90link link by default with the single
           precision library dismg.a. A double precision version
           of the library is also included (dismg_d.a). The C++
           library of Dislin (discpp.a) is only implemented as a
           double precision library.

        -  If the pre-compiled DISLIN Fortran 90 module file 
           'dislin.mod' is not compatible to your gfortran compiler
           version, you have to recompile the file 'dislin.f90' in the 
           DISLIN\gf directory with the command 'gfortran -c dislin.f90'.


3.)  Getting DISLIN Support

     The DISLIN distributions can be used freely without any restriction.
     Access to the source code is availabe via a support contract from the
     site www.dislin.de.

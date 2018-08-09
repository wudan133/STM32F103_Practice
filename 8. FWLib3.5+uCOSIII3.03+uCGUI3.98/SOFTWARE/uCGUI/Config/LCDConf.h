/*
*********************************************************************************************************
*                                             uC/GUI V3.98
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDConf_1375_C8_C320x240.h
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE           (240)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE           (320)   /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL    (16)

#define LCD_CONTROLLER      -1
#define LCD_FIXEDPALETTE    565
#define LCD_SWAP_RB         0
#define LCD_SWAP_XY         1
#define LCD_MIRROR_Y        1
#define LCD_MIRROR_X        0

#define LCD_INIT_CONTROLLER() ili9486_Initializtion(); 
 
#endif /* LCDCONF_H */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 

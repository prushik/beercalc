/*
 ***********************************************************************************
 *
 *	Bitterness  -  v1995.05.04
 *
 *	The Bitterness Calculator takes into account both the time of boil and the wort 
 *	gravity during the boil. The numbers are my own, based partly on data and partly
 *	on experience. Your experience and brewing practices may be different than mine
 *	so here's the official disclaimer: Your Mileage May Vary (YMMV).
 *
 *	 This one is for the US units page--IBU.html:
 *	
 *	(1.65*0.000125^(gravity-1))*(1-EXP(-0.04*time))*alpha*mass*28.3*1000
 *	---------------------------------------------------------------------
 *	                        (3.785*volume*4.15)
 *	
 *	and this one for the metric units page--ibumetric.html:
 *	
 *	(1.65*0.000125^(gravity-1))*(1-EXP(-0.04*time))*alpha*mass*1000
 *	-----------------------------------------------------------------
 *	                       (volume*4.15)
 *	
 *	The variables above are named exactly how they're named in the HTML.
 *
 *	input:
 *	version=US&volume=5&gravity=1.050&mass=2.0&alpha=5.0&time=60
 *
 *	To test this routine's POST method, do the following:
 *		setenv REQUEST_METHOD POST
 *		setenv CONTENT_TYPE "application/x-www-form-urlencoded"
 *		setenv CONTENT_LENGTH 60
 *		Bitterness.exe < input > output.html
 *
 *	To test this routine's GET method. do the following:
 *		setenv REQUEST_METHOD GET
 *		setenv QUERY_STRING "version=US&volume=5&gravity=1.050&mass=2.0&alpha=5.0&time=60"
 *		Bitterness.exe > output.html
 *
 ************************************************************************************
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "/usr/include/math.h"
#include <ctype.h>
#include <sys/stat.h>
#include <sys/fcntl.h>


#define	COEFF1				1.65
#define	COEFF2				1.25e-4
#define	COEFF3				0.04
#define	COEFF4				4.15
#define	GRAMSPEROUNCE		28.3
#define	LITERSPERGALLON		3.785
#define	GRAMSPERKG			1000.0

typedef enum {METRIC, US} units;
extern char *metric_unit_page[];
extern char *english_unit_page[];

void metric_ivars(volume, gravity, mass, alpha, time, bitterness)
float volume;
float gravity;
float mass;
float alpha;
float time;
float bitterness;
{
	char buffer[256];
	sprintf(buffer,"<LI>Boil Volume = <input  NAME=\"volume\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.1f\"> liters \n",volume);
	fputs(buffer,stdout);
				
	sprintf(buffer,"<LI>Boil wort gravity (specific gravity) = <input  NAME=\"gravity\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.3f\"> \n",gravity);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI>Mass of hops added = <input  NAME=\"mass\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.1f\"> grams \n",mass);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI>Alpha acid rating = <input  NAME=\"alpha\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.2f\"> percent \n",alpha*100.0);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI>Time hops were boiled = <input  NAME=\"time\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%5.1f\"> minutes \n",time);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI><B>Approximate Bittering Units = %.1f</B>\n", bitterness);
	fputs(buffer,stdout);
}

void english_ivars(volume, gravity, mass, alpha, time, bitterness)
float volume;
float gravity;
float mass;
float alpha;
float time;
float bitterness;
{
	char buffer[256];
	sprintf(buffer,"<LI>Boil Volume = <input  NAME=\"volume\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.1f\"> gallons \n",volume);
	fputs(buffer,stdout);
				
	sprintf(buffer,"<LI>Boil wort gravity (specific gravity) = <input  NAME=\"gravity\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.3f\"> \n",gravity);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI>Mass of hops added = <input  NAME=\"mass\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.1f\"> ounces \n",mass);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI>Alpha acid rating = <input  NAME=\"alpha\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.2f\"> percent \n",alpha*100.0);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI>Time hops were boiled = <input  NAME=\"time\" CHARSET=\"0123456789.\" SIZE=\"8\" MIN=\"1\" MAX=\"5\" MAXLENGTH=\"5\" VALUE=\"%.1f\"> minutes \n",time);
	fputs(buffer,stdout);

	sprintf(buffer,"<LI><B>Approximate Bittering Units = %5.1f</B>\n", bitterness);
	fputs(buffer,stdout);
}

void print_error(reason)
char *reason;
{
    printf("<TITLE>Error in Calculation.</TITLE>\n");
    printf("<H1>Error in Calculation.</H1>\n");
    printf(" %s.<P>\n",reason);
    exit(0);
}

/* Parse a Key=Value pairing, and look for '=' sign in ascending or descending order */
#define AscendingOrder	1
#define	DescendingOrder	2
char * parseKeyValuePair(string,order)
char *string;
int order;
{
	char *ptr = NULL;
	
	/* Find the equal sign and set ptr to next character. */
	if(order == AscendingOrder)
		ptr = (char *)(index(string,'=') + sizeof(char));
	else if(order == DescendingOrder)
		ptr = (char *)(rindex(string,'=') + sizeof(char));

	/* Advance ptr one character is present character is a space */
	if(ptr)
	{
		while(isspace(*ptr))
			ptr += sizeof(char);
	}
	return ptr;
}


int main(argc, argv)
int argc;
char **argv;
{
	int i;
	int line, nline;
	int nstr = 0;
	int len = 0;
	int lineno;
	char **page;
	char *ptr = NULL;
	char *method = NULL;
	char *string[7];
	char buffer[256];
	void (*ivars)();

	/* INPUT VARIABLES */
	float	alpha = -1.0;		/* Alpha acid rating (percent)			*/
	float	gravity = -1.0;		/* Boil wort gravity (specific gravity) */
	float	mass = -1.0;		/* Mass of hops added (grams/ounces)	*/
	float	time = -1.0;		/* Time hops were boiled (Minutes)		*/
	float	volume = -1.0;		/* Boil Volume (liters/gallons)			*/
#ifdef USUNITS
	units	version = US;		/* type of units, US or Metric			*/
#else
	units	version = METRIC;	/* type of units, US or Metric			*/
#endif

	/* OUTPUT VARIABLES */
	float	bitterness;			/* Bitterness result.					*/

#ifdef	DEBUG
	/* MIME HTML header */
	printf("Content-type: text/html\n\n");
#endif

	/* If the request method is GET, read the environment variable for data. */
	method = getenv("REQUEST_METHOD");
	if(!method) method = "UNKNOWN";
    if(strcmp(method,"GET")==0)
	{
    	ptr = getenv("QUERY_STRING");
    	if(ptr == NULL) {
        	printf("No query information to decode.\n");
        	exit(1);
    	}
		len = strlen(ptr);
		string[0] = (char *)malloc((len+1)*sizeof(char));
		strcpy(string[0], ptr);
    }
	/* If the request method is POST, read stdin for variable data. */
	else if(strcmp(method,"POST")==0)
	{
    	if(strcmp(getenv("CONTENT_TYPE"),"application/x-www-form-urlencoded")) {
        	printf("This script can only be used to decode form results. \n");
        	exit(1);
    	}
    	len = atoi(getenv("CONTENT_LENGTH"));
		string[0] = (char *)malloc((len+1)*sizeof(char));
		fread(string[0], sizeof(char), len, stdin);
		string[0][len] = '\0';
#ifdef DEBUG
		printf("REQUEST_METHOD = %s\n",method);
		printf("CONTENT_TYPE = %s\n", getenv("CONTENT_TYPE"));
		printf("CONTENT_LENGTH = %i\n", len);
#endif
	}
	else
	{
        printf("Unknown REQUEST_METHOD = %s.\n", method);
        exit(1);
	}
#ifdef	DEBUG
	printf("strlen(string[0]) = %i\n",len);
	printf("string[0] = %s\n",string[0]);
#endif

	/* Parse argument into "name=value" strings */
	nstr = 1;
	for(i=0; i<len; i++)
	{
		if(string[0][i] == '&')
		{
			string[0][i] = '\0';
			string[nstr++] = string[0] + (i+1)*sizeof(char);
		}
	}
#ifdef	DEBUG
	for(i=0; i<nstr; i++)
	{
		printf(" string[%i]: %s\n", i, string[i]);
	}
#endif


	/* Parse "name=value" strings */
	for(i=0; i<nstr; i++)
	{
		switch(*string[i])
		{
			/* Alpha = float */
			case 'a': case 'A':
			    ptr = parseKeyValuePair(string[i],AscendingOrder);
				if(ptr)
					alpha = atof(ptr)/100.0;
				else
				{
					sprintf(buffer, "Alpha '=' not found: arg=%i, string=%s\n", 
							i,string[i]);
					print_error(buffer);
				}
				break;
			
			/* Gravity = float */
			case 'g': case 'G':
			    ptr = parseKeyValuePair(string[i],AscendingOrder);
				if(ptr)
					gravity = atof(ptr);
				else
				{
					sprintf(buffer, "Gravity '=' not found: arg=%i, string=%s\n", 
							i,string[i]);
					print_error(buffer);
				}

				break;
			
			/* Mass = float */
			case 'm': case 'M':
			    ptr = parseKeyValuePair(string[i],AscendingOrder);
				if(ptr)
					mass = atof(ptr);
				else
				{
					sprintf(buffer, "Mass '=' not found: arg=%i, string=%s\n", 
							i,string[i]);
					print_error(buffer);
				}
				break;
			
			/* Time = float */
			case 't': case 'T':
			    ptr = parseKeyValuePair(string[i],AscendingOrder);
				if(ptr)
					time = atof(ptr);
				else
				{
					sprintf(buffer, "Time '=' not found: arg=%i, string=%s\n", 
							i,string[i]);
					print_error(buffer);
				}
				break;
			
			/* {Version,Volume} = {char,float} */
			case 'v': case 'V':
				switch(*(string[i]+sizeof(char)))
				{
					/* Version = char */
					case 'e': case 'E':
			    		ptr = parseKeyValuePair(string[i],DescendingOrder);
						switch(*ptr)
						{
							/* Version = US (english units) */
							case 'u': case 'U':
								version = US;
								break;
							/* Version = Metric units */
							case 'm': case 'M':
								version = METRIC;
								break;
							default:
								sprintf(buffer, "Unknown version: arg=%i, string=%s\n",
								       i,string[i]);
								print_error(buffer);
								break;
						}
						break;
					
					/* Volume = float */
					case 'o': case 'O':
			    		ptr = parseKeyValuePair(string[i],DescendingOrder);
						if(ptr)
							volume = atof(ptr);
						else
						{
							sprintf(buffer, "Volume '=' not found: arg=%i, string=%s\n", 
									i,string[i]);
							print_error(buffer);
						}
						break;
						
					default:
						sprintf(buffer, "Unknown 'V...' variable: arg=%i, string=%s\n", i,string[i]);
						print_error(buffer);
						break;
				}
				break;
				
			default:
				sprintf(buffer, "Unknown variable: arg=%i, string=%s\n",i,string[i]);
				print_error(buffer);
				break;
		}
	}

	
	/* Check for valid input */
#ifdef DEBUG
	printf("alpha = %f\n", alpha*100.0);
	printf("gravity = %f\n", gravity);
	printf("mass = %f\n", mass);
	printf("time = %f\n", time);
	printf("volume = %f\n", volume);
	printf("version = %i\n", version);
#endif
	
	/* Calculate bitterness value. */
	bitterness = COEFF1*pow(COEFF2,gravity-1.0) * 
				(1.0-exp(-COEFF3*time))*alpha*mass*GRAMSPERKG / 
				(volume*COEFF4);
	if(version == US) 
		bitterness *= (GRAMSPEROUNCE / LITERSPERGALLON);

#ifdef DEBUG
	printf("bitterness = %f\n", bitterness);
#endif

#ifndef	DEBUG
	/* MIME HTML header */
	printf("Content-type: text/html\n\n"); 
#endif

	/* Return html page with answer. */
	if(version == METRIC)
	{
		page = metric_unit_page;
		lineno = 39;
		ivars = metric_ivars;
	}
	else
	{
		page = english_unit_page;
		lineno = 39;
		ivars = english_ivars;
	}
	
	line = 0;
	while(page[line])
	{
		puts(page[line++]);
		if(line==lineno)
		{
			ivars(volume, gravity, mass, alpha, time, bitterness); 
		}
	}
	
	if(string[0]) free(string[0]);
	exit(0);
}



/************************ METRIC PAGE *************************************/
char *metric_unit_page[] = {
"",
"<!doctype html public \"-//W30//DTD W3 HTML 2.0//EN\">",
"",
"<HTML>",
"",
"<HEAD>",
"<TITLE>Hop Bitterness Calculator</TITLE>",
"</HEAD>",
"",
"<body bgcolor=\"#FFFFFF\" text=\"#000000\" link=\"#004000\" vlink=\"#500000\">",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"<H1>Bitterness Calculator</H1>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"The Bitterness Calculator takes into account both the time of boil",
"and the wort gravity during the boil. The numbers are my own, based ",
"partly on data and partly on experience. Your experience and brewing ",
"practices may be different than mine so here's the official disclaimer:",
"Your Mileage May Vary (YMMV). Have fun with this and don't forget to ",
"let me know what you think of it.</P>",
"",
"Fill out the form below to find out the estimated Bittering ",
"Units (BU) in your homebrew. When you finish entering all",
"the values click <B>Calculate</B> to send them to the calculator ",
"program.<P>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"<H3>Use <A HREF=\"http://realbeer.com/hops/IBU.html\">US units</A> instead</H3><P>",
"",
"<form ACTION=\"http://realbeer.com/cgi-bin/hops/Bitterness.exe\" METHOD=\"POST\">",
"",
"<INPUT TYPE=\"hidden\" NAME=\"version\" VALUE=\"METRIC\">",
"<H3>Please enter the following values:</H3><P>",
"",
"<UL>",
"</UL>",
"",
"<input TYPE=\"SUBMIT\" VALUE=\"Calculate\">",
"<input TYPE=\"RESET\" VALUE=\"Start Over\">",
"<P>",
"",
"</FORM>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"<ADDRESS>",
"Send feedback to: gtinseth (at) yahoo (dot) com",
"</ADDRESS>",
"<BR>",
"<a href=http://realbeer.com/><img height=108 width=108 src=/rbp/rbp.homebutton.gif></a>",
"<a href=/hops/><img height=108 width=108 src=/rbp/buttons/hops.button.GIF></a><P>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"",
"</BODY>",
"",
"</HTML>",
NULL};

/************************ US UNITS PAGE *************************************/
char *english_unit_page[] = {
"",
"<!doctype html public \"-//W30//DTD W3 HTML 2.0//EN\">",
"",
"<HTML>",
"",
"<HEAD>",
"<TITLE>Hop Bitterness Calculator</TITLE>",
"</HEAD>",
"",
"<body bgcolor=\"#FFFFFF\" text=\"#000000\" link=\"#004000\" vlink=\"#500000\">",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"<H1>Bitterness Calculator</H1>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"The Bitterness Calculator takes into account both the time of boil",
"and the wort gravity during the boil. The numbers are my own, based ",
"partly on data and partly on experience. Your experience and brewing ",
"practices may be different than mine so here's the official disclaimer:",
"Your Mileage May Vary (YMMV). Have fun with this and don't forget to ",
"let me know what you think of it.</P>",
"",
"Fill out the form below to find out the estimated Bittering ",
"Units (BU) in your homebrew. When you finish entering all",
"the values click <B>Calculate</B> to send them to the calculator ",
"program.<P>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"<H3>Use <A HREF=\"http://realbeer.com/hops/ibumetric.html\">metric units</A> instead</H3><P>",
"",
"<form ACTION=\"http://realbeer.com/cgi-bin/hops/Bitterness.exe\" METHOD=\"POST\">",
"",
"<INPUT TYPE=\"hidden\" NAME=\"version\" VALUE=\"US\">",
"<H3>Please enter the following values:</H3><P>",
"",
"<UL>",
"</UL>",
"",
"<input TYPE=\"SUBMIT\" VALUE=\"Calculate\">",
"<input TYPE=\"RESET\" VALUE=\"Start Over\">",
"<P>",
"",
"</FORM>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"<ADDRESS>",
"Send feedback to: gtinseth (at) yahoo (dot) com",
"</ADDRESS>",
"<BR>",
"<a href=http://realbeer.com/><img height=108 width=108 src=/rbp/rbp.homebutton.gif></a>",
"<a href=/hops/><img height=108 width=108 src=/rbp/buttons/hops.button.GIF></a><P>",
"",
"<IMG ALIGN=bottom SRC=/hops/pics/greenblackline.gif><P>",
"",
"",
"</BODY>",
"",
"</HTML>",
NULL};



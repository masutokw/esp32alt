
#line 1 "command.rl"
/*
 * Parses LX200 protocol you must process this file with RAGEL compiler to get command.cpp file
 */

#define ADD_DIGIT(var,digit) var=var*10+digit-'0';
#define APPEND strcat(response,tmessage);
#define SYNC_MESSAGE "sync#"
//#define SYNC_MESSAGE "Coordinates     matched.        #"

#include <string.h>
#include <stdio.h>
#include "mount.h"
#include "misc.h"
#include <math.h>
#include "tb6612.h"

char response [200];
char tmessage[50];
extern c_star st_now, st_target, st_current;
extern char volatile sync_target;
extern stepper focus_motor;
struct _telescope_
{   long dec_target,ra_target;
    long alt_target,az_target;
    long current_dec,current_ra;
    long lat,longitude,h_secs;
    int day,month,year,dayofyear;
    int hour,min,sec;

}
mount;
extern long sdt_millis;
extern mount_t *telescope;

void lxprintsite(void)
{
    sprintf(tmessage,"Site Name#");APPEND;
};


void set_cmd_exe(char cmd,long date)
{ int temp ;
    switch (cmd)
    {
    case 'r':
       if (telescope->mount_mode) mount.ra_target=date;
	   else
	    telescope->azmotor->target=telescope->ra_target=date*SEC_TO_RAD*15.0;
        break;
    case 'd':
        if (telescope->mount_mode) mount.dec_target=date;
		else
		{  telescope->dec_target=date*SEC_TO_RAD;
        if  (telescope->dec_target<0.0)
            telescope->altmotor->target=2*M_PI+telescope->dec_target;
        else
            telescope->altmotor->target=telescope->dec_target;}
        break;
    case 'a':
        mount.alt_target=date;
        break;
    case 'z':
        mount.az_target=date ;
        break;
    case 't':
        mount.lat=date ;
        telescope->lat=date/3600.0;

        break;
    case 'g':
        mount.longitude=date ;
		if (date <648000)
        telescope->longitude=-date/3600.0; else
		telescope->longitude=(1296000.0-date)/3600.0;

        break;
    case 'L' :
        mount.h_secs=date;
        mount.hour=date/3600;
        temp = (date % 3600);
        mount.min = temp / 60;
        mount.sec = temp % 60;
        //setclock (mount.year,mount.month,mount.day,mount.hour,mount.min,mount.sec,telescope->time_zone)
        setclock (22,8,01,14,6,12,telescope->time_zone);
        break;
    case 'S':
        break;

    }
}
void set_date( int day,int month,int year)
{   mount.month=month-1;
    mount.day=day;
    mount.year=100+year;
    setclock (mount.year,mount.month,mount.day,mount.hour,mount.min,mount.sec,telescope->time_zone);
    if (telescope->mount_mode == EQ) {
    sdt_init(telescope->longitude, telescope->time_zone);
    }
  else
  { telescope->is_tracking = FALSE;
    sync_target = TRUE;
    tak_init(telescope);
    telescope->azmotor->targetspeed=0.0;
    telescope->altmotor->targetspeed=0.0;
  }
	sprintf(tmessage,"%cUpdating Planetary Data#     #",'1');APPEND;
}
void set_time( int hour,int min,int sec)
{
    mount.min=min;
    mount.hour=hour;
    mount.sec=sec;
    sprintf(tmessage,"1");APPEND;
}

//----------------------------------------------------------------------------------------
long command( char *str )
{
    char *p = str, *pe = str + strlen( str );
    int cs;
    char stcmd;
    long deg=0;
    int min=0;
    int sec=0;
    int neg = 1;
    tmessage[0]=0;
    response[0]=0;
	int pulse=0;
	int focus_counter=0;
    
#line 2 "command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 1, 17, 1, 18, 1, 
	20, 1, 21, 1, 22, 1, 23, 1, 
	24, 1, 25, 1, 28, 1, 29, 1, 
	30, 1, 32, 1, 33, 1, 34, 1, 
	35, 1, 36, 1, 37, 1, 38, 1, 
	39, 1, 40, 1, 41, 2, 1, 26, 
	2, 2, 27, 2, 31, 19
};

static const short _command_key_offsets[] = {
	0, 0, 1, 9, 10, 12, 13, 20, 
	21, 22, 24, 26, 28, 30, 32, 34, 
	35, 36, 37, 39, 41, 43, 45, 47, 
	49, 50, 52, 54, 56, 58, 60, 62, 
	63, 64, 65, 78, 79, 80, 81, 82, 
	83, 84, 85, 86, 87, 88, 89, 90, 
	96, 97, 98, 102, 104, 106, 108, 110, 
	111, 116, 117, 121, 124, 125, 135, 140, 
	142, 144, 145, 147, 149, 150, 152, 154, 
	155, 160, 162, 164, 168, 170, 171, 173, 
	178, 180, 182, 183, 185, 187, 188, 190, 
	192, 193, 198, 200, 202, 204, 206, 208, 
	211, 213, 214, 216, 218, 220, 222, 225, 
	232, 239, 241, 243, 253, 255, 257, 258, 
	266, 267, 268, 271
};

static const char _command_trans_keys[] = {
	58, 67, 70, 71, 77, 81, 82, 83, 
	112, 77, 35, 82, 35, 43, 45, 65, 
	76, 80, 81, 112, 35, 35, 43, 45, 
	48, 57, 48, 57, 48, 57, 48, 57, 
	48, 57, 35, 83, 49, 43, 45, 48, 
	57, 48, 57, 48, 57, 48, 57, 48, 
	57, 35, 43, 45, 48, 57, 48, 57, 
	48, 57, 48, 57, 48, 57, 35, 35, 
	35, 65, 67, 68, 71, 76, 77, 82, 
	83, 90, 100, 103, 114, 116, 35, 35, 
	35, 35, 35, 35, 35, 35, 35, 35, 
	35, 35, 83, 101, 103, 110, 115, 119, 
	35, 35, 101, 110, 115, 119, 48, 57, 
	48, 57, 48, 57, 48, 57, 35, 35, 
	101, 110, 115, 119, 35, 67, 71, 77, 
	83, 35, 48, 52, 35, 67, 71, 76, 
	83, 97, 100, 103, 114, 116, 122, 32, 
	9, 13, 48, 57, 48, 57, 48, 57, 
	47, 48, 57, 48, 57, 47, 48, 57, 
	48, 57, 35, 32, 43, 45, 9, 13, 
	43, 45, 48, 57, 35, 46, 48, 57, 
	48, 57, 35, 35, 46, 32, 9, 13, 
	48, 57, 48, 57, 48, 57, 58, 48, 
	57, 48, 57, 58, 48, 57, 48, 57, 
	35, 32, 9, 13, 48, 50, 48, 50, 
	48, 57, 47, 58, 48, 53, 48, 57, 
	46, 47, 58, 48, 57, 35, 48, 53, 
	48, 57, 35, 46, 48, 57, 35, 48, 
	57, 32, 43, 45, 9, 13, 48, 57, 
	32, 43, 45, 9, 13, 48, 57, 48, 
	57, 48, 57, 33, 47, 48, 57, 58, 
	64, 91, 96, 123, 126, 48, 53, 48, 
	57, 35, 33, 47, 58, 64, 91, 96, 
	123, 126, 72, 35, 6, 35, 58, 6, 
	35, 58, 48, 53, 0
};

static const char _command_single_lengths[] = {
	0, 1, 8, 1, 2, 1, 7, 1, 
	1, 2, 0, 0, 0, 0, 0, 1, 
	1, 1, 2, 0, 0, 0, 0, 0, 
	1, 2, 0, 0, 0, 0, 0, 1, 
	1, 1, 13, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 6, 
	1, 1, 4, 0, 0, 0, 0, 1, 
	5, 1, 4, 1, 1, 10, 1, 0, 
	0, 1, 0, 0, 1, 0, 0, 1, 
	3, 2, 0, 2, 0, 1, 2, 1, 
	0, 0, 1, 0, 0, 1, 0, 0, 
	1, 1, 0, 0, 2, 0, 0, 3, 
	0, 1, 0, 0, 2, 0, 1, 3, 
	3, 0, 0, 0, 0, 0, 1, 0, 
	1, 1, 3, 3
};

static const char _command_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 1, 1, 1, 1, 0, 
	0, 0, 0, 1, 1, 1, 1, 1, 
	0, 0, 1, 1, 1, 1, 1, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 1, 1, 1, 1, 0, 
	0, 0, 0, 1, 0, 0, 2, 1, 
	1, 0, 1, 1, 0, 1, 1, 0, 
	1, 0, 1, 1, 1, 0, 0, 2, 
	1, 1, 0, 1, 1, 0, 1, 1, 
	0, 2, 1, 1, 0, 1, 1, 0, 
	1, 0, 1, 1, 0, 1, 1, 2, 
	2, 1, 1, 5, 1, 1, 0, 4, 
	0, 0, 0, 1
};

static const short _command_index_offsets[] = {
	0, 0, 2, 11, 13, 16, 18, 26, 
	28, 30, 33, 35, 37, 39, 41, 43, 
	45, 47, 49, 52, 54, 56, 58, 60, 
	62, 64, 67, 69, 71, 73, 75, 77, 
	79, 81, 83, 97, 99, 101, 103, 105, 
	107, 109, 111, 113, 115, 117, 119, 121, 
	128, 130, 132, 137, 139, 141, 143, 145, 
	147, 153, 155, 160, 163, 165, 176, 180, 
	182, 184, 186, 188, 190, 192, 194, 196, 
	198, 203, 206, 208, 212, 214, 216, 219, 
	223, 225, 227, 229, 231, 233, 235, 237, 
	239, 241, 245, 247, 249, 252, 254, 256, 
	260, 262, 264, 266, 268, 271, 273, 276, 
	282, 288, 290, 292, 298, 300, 302, 304, 
	309, 311, 313, 317
};

static const unsigned char _command_indicies[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 
	8, 9, 1, 10, 1, 11, 12, 1, 
	11, 1, 13, 14, 15, 16, 17, 18, 
	19, 1, 20, 1, 21, 1, 22, 23, 
	1, 24, 1, 25, 1, 26, 1, 27, 
	1, 28, 1, 29, 1, 30, 1, 31, 
	1, 32, 33, 1, 34, 1, 35, 1, 
	36, 1, 37, 1, 38, 1, 39, 1, 
	40, 41, 1, 42, 1, 43, 1, 44, 
	1, 45, 1, 46, 1, 47, 1, 48, 
	1, 49, 1, 50, 51, 52, 53, 54, 
	55, 56, 54, 57, 58, 59, 60, 61, 
	1, 62, 1, 63, 1, 64, 1, 65, 
	1, 66, 1, 67, 1, 68, 1, 69, 
	1, 70, 1, 71, 1, 72, 1, 73, 
	1, 74, 75, 76, 75, 75, 75, 1, 
	77, 1, 78, 1, 79, 79, 79, 79, 
	1, 80, 1, 81, 1, 82, 1, 83, 
	1, 84, 1, 85, 86, 86, 86, 86, 
	1, 85, 1, 87, 87, 87, 87, 1, 
	88, 89, 1, 88, 1, 90, 91, 92, 
	93, 94, 94, 94, 93, 94, 94, 1, 
	95, 95, 96, 1, 96, 1, 97, 1, 
	98, 1, 99, 1, 100, 1, 101, 1, 
	102, 1, 103, 1, 104, 1, 105, 106, 
	107, 105, 1, 106, 107, 1, 108, 1, 
	109, 110, 111, 1, 112, 1, 109, 1, 
	109, 110, 1, 113, 113, 114, 1, 114, 
	1, 115, 1, 116, 1, 117, 1, 118, 
	1, 119, 1, 120, 1, 121, 1, 122, 
	1, 123, 123, 124, 1, 124, 1, 125, 
	1, 126, 126, 1, 127, 1, 128, 1, 
	129, 130, 130, 1, 131, 1, 132, 1, 
	133, 1, 134, 1, 132, 135, 1, 136, 
	1, 132, 137, 1, 138, 139, 140, 138, 
	141, 1, 139, 139, 140, 139, 141, 1, 
	141, 1, 142, 1, 143, 144, 143, 143, 
	143, 1, 145, 1, 146, 1, 147, 130, 
	143, 143, 143, 143, 1, 148, 1, 149, 
	1, 150, 151, 0, 1, 150, 151, 0, 
	133, 1, 0
};

static const char _command_trans_targs[] = {
	2, 0, 3, 6, 34, 47, 56, 58, 
	61, 112, 4, 114, 5, 7, 8, 9, 
	16, 25, 32, 33, 114, 114, 10, 10, 
	11, 12, 13, 14, 15, 114, 17, 18, 
	19, 19, 20, 21, 22, 23, 24, 114, 
	26, 26, 27, 28, 29, 30, 31, 114, 
	114, 114, 35, 36, 37, 38, 39, 40, 
	41, 42, 43, 44, 45, 46, 114, 114, 
	114, 114, 114, 114, 114, 114, 114, 114, 
	114, 114, 48, 49, 50, 114, 114, 51, 
	52, 53, 54, 55, 114, 114, 57, 59, 
	114, 60, 62, 72, 79, 89, 103, 63, 
	64, 65, 66, 67, 68, 69, 70, 71, 
	114, 73, 74, 74, 75, 114, 76, 78, 
	77, 80, 81, 82, 83, 84, 85, 86, 
	87, 88, 114, 90, 91, 92, 93, 94, 
	95, 96, 98, 97, 114, 99, 100, 101, 
	102, 97, 104, 105, 105, 106, 107, 108, 
	111, 109, 110, 115, 113, 114, 1, 1
};

static const char _command_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 43, 0, 0, 0, 0, 
	0, 0, 0, 0, 63, 61, 0, 11, 
	9, 9, 9, 9, 9, 67, 0, 0, 
	0, 11, 9, 9, 9, 9, 9, 71, 
	0, 11, 9, 9, 9, 9, 9, 65, 
	69, 73, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 29, 35, 
	25, 57, 47, 37, 23, 27, 33, 39, 
	31, 41, 0, 51, 0, 17, 13, 51, 
	7, 7, 7, 7, 15, 19, 51, 51, 
	21, 0, 0, 0, 0, 51, 51, 0, 
	1, 1, 0, 3, 3, 0, 5, 5, 
	53, 0, 0, 11, 1, 83, 0, 1, 
	0, 0, 1, 1, 0, 3, 3, 0, 
	5, 5, 59, 0, 1, 1, 0, 3, 
	77, 0, 0, 45, 49, 5, 80, 0, 
	0, 0, 0, 0, 11, 1, 1, 0, 
	1, 3, 77, 49, 0, 75, 55, 0
};

static const int command_start = 114;
static const int command_first_final = 114;
static const int command_error = 0;

static const int command_en_main = 114;


#line 133 "command.rl"





    
#line 220 "command.cpp"
	{
	cs = command_start;
	}

#line 223 "command.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _command_trans_keys + _command_key_offsets[cs];
	_trans = _command_index_offsets[cs];

	_klen = _command_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _command_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _command_indicies[_trans];
	cs = _command_trans_targs[_trans];

	if ( _command_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _command_actions + _command_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 140 "command.rl"
	{ADD_DIGIT(deg,(*p)); }
	break;
	case 1:
#line 141 "command.rl"
	{ADD_DIGIT(min,(*p)); }
	break;
	case 2:
#line 142 "command.rl"
	{ADD_DIGIT(sec,(*p)); }
	break;
	case 3:
#line 143 "command.rl"
	{ADD_DIGIT(pulse,(*p));}
	break;
	case 4:
#line 144 "command.rl"
	{ADD_DIGIT(focus_counter,(*p));}
	break;
	case 5:
#line 145 "command.rl"
	{ neg=-1;}
	break;
	case 6:
#line 146 "command.rl"
	{mount_move(telescope,stcmd);}
	break;
	case 7:
#line 147 "command.rl"
	{pulse_guide(telescope,stcmd,pulse);}
	break;
	case 8:
#line 148 "command.rl"
	{if (telescope->mount_mode)
					{goto_ra_dec(telescope,mount.ra_target*15.0*SEC_TO_RAD,mount.dec_target*SEC_TO_RAD);}
					 else mount_slew(telescope);
					 sprintf(tmessage,"0");APPEND;}
	break;
	case 9:
#line 152 "command.rl"
	{mount_stop(telescope,stcmd);}
	break;
	case 10:
#line 153 "command.rl"
	{select_rate(telescope,stcmd); }
	break;
	case 11:
#line 154 "command.rl"
	{if (telescope->mount_mode) lxprintra1(tmessage, st_current.ra);
							else mount_lxra_str(tmessage,telescope); APPEND;}
	break;
	case 12:
#line 156 "command.rl"
	{if (telescope->mount_mode) lxprintde1(tmessage, st_current.dec);
							else mount_lxde_str(tmessage,telescope); APPEND;}
	break;
	case 13:
#line 158 "command.rl"
	{ lxprintaz1(tmessage, st_current.az); APPEND;}
	break;
	case 14:
#line 159 "command.rl"
	{lxprintde1(tmessage, st_current.alt); APPEND;}
	break;
	case 15:
#line 160 "command.rl"
	{ lxprintra1(tmessage, st_target.ra); APPEND;}
	break;
	case 16:
#line 161 "command.rl"
	{lxprintde1(tmessage, st_target.dec); APPEND;}
	break;
	case 17:
#line 162 "command.rl"
	{lxprintdate1(tmessage);APPEND;}
	break;
	case 18:
#line 163 "command.rl"
	{ lxprintsite();}
	break;
	case 19:
#line 164 "command.rl"
	{sprintf(tmessage,"1");APPEND;deg=sec=min=0;}
	break;
	case 20:
#line 165 "command.rl"
	{lxprintlong1(tmessage,telescope->longitude);APPEND;}
	break;
	case 21:
#line 166 "command.rl"
	{lxprintlat1(tmessage,telescope->lat);APPEND;}
	break;
	case 22:
#line 169 "command.rl"
	{if (telescope->mount_mode)
						align_sync_all(telescope,mount.ra_target,mount.dec_target);
						else
						{sync_eq(telescope);telescope->altmotor->slewing= telescope->azmotor->slewing=FALSE;}
						sprintf(tmessage,"sync#");APPEND;
						}
	break;
	case 23:
#line 175 "command.rl"
	{deg+=((*p)-'0')*6;}
	break;
	case 24:
#line 176 "command.rl"
	{ lxprinttime1(tmessage);APPEND;}
	break;
	case 25:
#line 177 "command.rl"
	{set_cmd_exe(stcmd,(neg*(deg )));
                             sprintf(tmessage,"1");APPEND;deg=sec=min=0;
                            }
	break;
	case 26:
#line 180 "command.rl"
	{deg=deg*3600+min*60;}
	break;
	case 27:
#line 181 "command.rl"
	{deg+=sec;}
	break;
	case 28:
#line 182 "command.rl"
	{stcmd=(*p);}
	break;
	case 29:
#line 183 "command.rl"
	{set_date(min,deg,sec);}
	break;
	case 30:
#line 184 "command.rl"
	{if (telescope->mount_mode==ALTAZ) sprintf(tmessage,"A");else sprintf(tmessage,"P") ; APPEND; }
	break;
	case 31:
#line 185 "command.rl"
	{ telescope->time_zone=deg;}
	break;
	case 32:
#line 186 "command.rl"
	{lxprintGMT_offset(tmessage,telescope->time_zone );APPEND}
	break;
	case 33:
#line 187 "command.rl"
	{set_time(deg,min,sec);}
	break;
	case 34:
#line 188 "command.rl"
	{move_to(&focus_motor,0);}
	break;
	case 35:
#line 189 "command.rl"
	{move_to(&focus_motor,focus_motor.max_steps);}
	break;
	case 36:
#line 190 "command.rl"
	{move_to(&focus_motor,focus_motor.position+(focus_counter*neg));}
	break;
	case 37:
#line 191 "command.rl"
	{move_to(&focus_motor,focus_counter);}
	break;
	case 38:
#line 192 "command.rl"
	{move_to(&focus_motor,focus_motor.position);}
	break;
	case 39:
#line 193 "command.rl"
	{focus_motor.position=focus_motor.target=focus_counter;}
	break;
	case 40:
#line 194 "command.rl"
	{sprintf(tmessage,"%05d#",focus_motor.position);APPEND;}
	break;
	case 41:
#line 195 "command.rl"
	{mount_home_set(telescope);}
	break;
#line 434 "command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 244 "command.rl"


//---------------------------------------------------------------------------------------------------------------------
    if ( cs < command_first_final )
        //	fprintf( stderr, "LX command:  error\n" );

        return  neg;
};




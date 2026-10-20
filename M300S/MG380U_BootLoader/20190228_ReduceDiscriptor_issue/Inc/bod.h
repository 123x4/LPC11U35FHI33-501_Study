#ifndef __BOD_H__
#define __BOD_H__


typedef enum bod_rst_level {

	bod_rst_1_46V,  /*!< Brown-out reset at 1.46v */
	bod_rst_2_06V,	/*!< Brown-out reset at 2.06v */
	bod_rst_2_35V,	/*!< Brown-out reset at 2.35v */
	bod_rst_2_63V,	/*!< Brown-out reset at 2.63v */

} bod_rst_level_t;

typedef enum bod_int_level {

	bod_int_reserved,  /* reserved */
	bod_int_2_22V,	/*!< Brown-out interrupt at 2.22v */
	bod_int_2_52V,	/*!< Brown-out interrupt at 2.52v */
	bod_int_2_80V,	/*!< Brown-out interrupt at 2.80v */

} bod_int_level_t;


extern void bod_set_level( bod_rst_level_t rst_level, bod_int_level_t int_level );
//extern void bod_enable();
void bod_enable(void);
extern void Clear_reset_status(uint32_t reset);



#endif

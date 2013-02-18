/*!The Treasure Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2012, ruki All rights reserved.
 *
 * @author		ruki
 * @file		number.c
 * @ingroup 	object
 *
 */

/* ///////////////////////////////////////////////////////////////////////
 * trace
 */
//#define TB_TRACE_IMPL_TAG 		"object"

/* ///////////////////////////////////////////////////////////////////////
 * includes
 */
#include "object.h"

/* ///////////////////////////////////////////////////////////////////////
 * types
 */

// the number type
typedef struct __tb_number_t
{
	// the object base
	tb_object_t 		base;

	// the number type
	tb_size_t 			type;

	// the number value
	union
	{
		// the uint8
		tb_uint8_t 		u8;
	
		// the sint8
		tb_sint8_t 		s8;
	
		// the uint16
		tb_uint16_t 	u16;
	
		// the sint16
		tb_sint16_t 	s16;
	
		// the uint32
		tb_uint32_t 	u32;
	
		// the sint32
		tb_sint32_t 	s32;
	
		// the uint64
		tb_uint64_t 	u64;
	
		// the sint64
		tb_sint64_t 	s64;
	
		// the float
		tb_float_t 		f;
	
		// the double
		tb_double_t 	d;
	
	}v;

}tb_number_t;

/* ///////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ tb_number_t* tb_number_cast(tb_object_t* object)
{
	// check
	tb_assert_and_check_return_val(object && object->type == TB_OBJECT_TYPE_NUMBER, tb_null);

	// cast
	return (tb_number_t*)object;
}
static tb_object_t* tb_number_copy(tb_object_t* object)
{
	// check
	tb_number_t* number = (tb_number_t*)object;
	tb_assert_and_check_return_val(number, tb_null);

	// copy
	switch (number->type)
	{
	case TB_NUMBER_TYPE_UINT64:
		return tb_number_init_from_uint64(number->v.u64);
	case TB_NUMBER_TYPE_SINT64:
		return tb_number_init_from_sint64(number->v.s64);
	case TB_NUMBER_TYPE_UINT32:
		return tb_number_init_from_uint32(number->v.u32);
	case TB_NUMBER_TYPE_SINT32:
		return tb_number_init_from_sint32(number->v.s32);
	case TB_NUMBER_TYPE_UINT16:
		return tb_number_init_from_uint16(number->v.u16);
	case TB_NUMBER_TYPE_SINT16:
		return tb_number_init_from_sint16(number->v.s16);
	case TB_NUMBER_TYPE_UINT8:
		return tb_number_init_from_uint8(number->v.u8);
	case TB_NUMBER_TYPE_SINT8:
		return tb_number_init_from_sint8(number->v.s8);
	case TB_NUMBER_TYPE_FLOAT:
		return tb_number_init_from_float(number->v.f);
	case TB_NUMBER_TYPE_DOUBLE:
		return tb_number_init_from_double(number->v.d);
	default:
		break;
	}

	return tb_null;
}
static tb_void_t tb_number_exit(tb_object_t* object)
{
	if (object) tb_free(object);
}
static tb_void_t tb_number_cler(tb_object_t* object)
{
	// check
	tb_number_t* number = (tb_number_t*)object;
	tb_assert_and_check_return(number);

	// cler
	switch (number->type)
	{
	case TB_NUMBER_TYPE_UINT64:
		number->v.u64 = 0;
		break;
	case TB_NUMBER_TYPE_SINT64:
		number->v.s64 = 0;
		break;
	case TB_NUMBER_TYPE_UINT32:
		number->v.u32 = 0;
		break;
	case TB_NUMBER_TYPE_SINT32:
		number->v.s32 = 0;
		break;
	case TB_NUMBER_TYPE_UINT16:
		number->v.u16 = 0;
		break;
	case TB_NUMBER_TYPE_SINT16:
		number->v.s16 = 0;
		break;
	case TB_NUMBER_TYPE_UINT8:
		number->v.u8 = 0;
		break;
	case TB_NUMBER_TYPE_SINT8:
		number->v.s8 = 0;
		break;
	case TB_NUMBER_TYPE_FLOAT:
		number->v.f = 0.;
		break;
	case TB_NUMBER_TYPE_DOUBLE:
		number->v.d = 0.;
		break;
	default:
		break;
	}
}
static tb_number_t* tb_number_init_base()
{
	// make
	tb_number_t* number = tb_malloc0(sizeof(tb_number_t));
	tb_assert_and_check_return_val(number, tb_null);

	// init object
	if (!tb_object_init(number, TB_OBJECT_FLAG_NONE, TB_OBJECT_TYPE_NUMBER)) goto fail;

	// init base
	number->base.copy = tb_number_copy;
	number->base.cler = tb_number_cler;
	number->base.exit = tb_number_exit;

	// ok
	return number;

	// no
fail:
	if (number) tb_free(number);
	return tb_null;
}
static tb_object_t* tb_number_read_xml(tb_handle_t reader, tb_size_t event)
{
	// check
	tb_assert_and_check_return_val(reader && event, tb_null);

	// empty?
	if (event == TB_XML_READER_EVENT_ELEMENT_EMPTY) 
		return tb_number_init_from_uint32(0);

	// walk
	tb_object_t* number = tb_null;
	while (event = tb_xml_reader_next(reader))
	{
		switch (event)
		{
		case TB_XML_READER_EVENT_ELEMENT_END: 
			{
				// name
				tb_char_t const* name = tb_xml_reader_element(reader);
				tb_assert_and_check_goto(name, end);
				
				// is end?
				if (!tb_stricmp(name, "number")) goto end;
			}
			break;
		case TB_XML_READER_EVENT_TEXT: 
			{
				// text
				tb_char_t const* text = tb_xml_reader_text(reader);
				tb_assert_and_check_goto(text, end);
				tb_trace_impl("number: %s", text);

				// has sign? is float?
				tb_size_t s = 0;
				tb_size_t f = 0;
				tb_char_t const* p = text;
				for (; *p; p++)
				{
					if (!s && *p == '-') s = 1;
					if (!f && *p == '.') f = 1;
					if (s && f) break;
				}
				
				// number
				if (f) number = tb_number_init_from_double(tb_atof(text));
				else number = s? tb_number_init_from_sint64(tb_stoi64(text)) : tb_number_init_from_uint64(tb_stou64(text));
				tb_assert_and_check_goto(number, end);
			}
			break;
		default:
			break;
		}
	}

end:

	// ok?
	return number;
}
static tb_bool_t tb_number_writ_xml(tb_object_t* object, tb_gstream_t* gst, tb_size_t level)
{
	// writ
	switch (tb_number_type(object))
	{
	case TB_NUMBER_TYPE_UINT64:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%llu</number>\n", tb_number_uint64(object));
		break;
	case TB_NUMBER_TYPE_SINT64:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%lld</number>\n", tb_number_sint64(object));
		break;
	case TB_NUMBER_TYPE_UINT32:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%u</number>\n", tb_number_uint32(object));
		break;
	case TB_NUMBER_TYPE_SINT32:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%d</number>\n", tb_number_sint32(object));
		break;
	case TB_NUMBER_TYPE_UINT16:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%u</number>\n", tb_number_uint16(object));
		break;
	case TB_NUMBER_TYPE_SINT16:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%d</number>\n", tb_number_sint16(object));
		break;
	case TB_NUMBER_TYPE_UINT8:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%u</number>\n", tb_number_uint8(object));
		break;
	case TB_NUMBER_TYPE_SINT8:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%d</number>\n", tb_number_sint8(object));
		break;
	case TB_NUMBER_TYPE_FLOAT:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%f</number>\n", tb_number_float(object));
		break;
	case TB_NUMBER_TYPE_DOUBLE:
		tb_object_writ_tab(gst, level);
		tb_gstream_printf(gst, "<number>%lf</number>\n", tb_number_double(object));
		break;
	default:
		break;
	}

	// ok
	return tb_true;
}
static tb_object_t* tb_number_read_bin(tb_gstream_t* gst, tb_size_t type, tb_size_t size)
{
	tb_trace_noimpl();
	return tb_null;
}
static tb_bool_t tb_number_writ_bin(tb_object_t* object, tb_gstream_t* gst)
{
	// writ type & size
	if (!tb_object_writ_bin_type_size(gst, object->type, 0xf)) return tb_false;

	// ok
	return tb_true;
}
/* ///////////////////////////////////////////////////////////////////////
 * interfaces
 */
tb_bool_t tb_number_init_reader()
{
	if (!tb_object_set_xml_reader("number", tb_number_read_xml)) return tb_false;
	if (!tb_object_set_bin_reader(TB_OBJECT_TYPE_NUMBER, tb_number_read_bin)) return tb_false;
	return tb_true;
}
tb_bool_t tb_number_init_writer()
{
	if (!tb_object_set_xml_writer(TB_OBJECT_TYPE_NUMBER, tb_number_writ_xml)) return tb_false;
	if (!tb_object_set_bin_writer(TB_OBJECT_TYPE_NUMBER, tb_number_writ_bin)) return tb_false;
	return tb_true;
}
tb_object_t* tb_number_init_from_uint8(tb_uint8_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_UINT8;
	number->v.u8 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_sint8(tb_sint8_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_SINT8;
	number->v.s8 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_uint16(tb_uint16_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_UINT16;
	number->v.u16 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_sint16(tb_sint16_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_SINT16;
	number->v.s16 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_uint32(tb_uint32_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_UINT32;
	number->v.u32 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_sint32(tb_sint32_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_SINT32;
	number->v.s32 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_uint64(tb_uint64_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_UINT64;
	number->v.u64 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_sint64(tb_sint64_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_SINT64;
	number->v.s64 = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_float(tb_float_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_FLOAT;
	number->v.f = value;

	// ok
	return number;
}

tb_object_t* tb_number_init_from_double(tb_double_t value)
{
	// make
	tb_number_t* number = tb_number_init_base();
	tb_assert_and_check_return_val(number, tb_null);

	// init value
	number->type = TB_NUMBER_TYPE_DOUBLE;
	number->v.d = value;

	// ok
	return number;
}

tb_size_t tb_number_type(tb_object_t* object)
{
	// check
	tb_number_t* number = tb_number_cast(object);
	tb_assert_and_check_return_val(number, TB_NUMBER_TYPE_NONE);

	// type
	return number->type;
}

tb_uint8_t tb_number_uint8(tb_object_t* object)
{
	return (tb_uint8_t)tb_number_uint64(object);
}
tb_sint8_t tb_number_sint8(tb_object_t* object)
{
	return (tb_sint8_t)tb_number_sint64(object);
}
tb_uint16_t tb_number_uint16(tb_object_t* object)
{
	return (tb_uint16_t)tb_number_uint64(object);
}
tb_sint16_t tb_number_sint16(tb_object_t* object)
{
	return (tb_sint16_t)tb_number_sint64(object);
}
tb_uint32_t tb_number_uint32(tb_object_t* object)
{
	return (tb_uint32_t)tb_number_uint64(object);
}
tb_sint32_t tb_number_sint32(tb_object_t* object)
{
	return (tb_sint32_t)tb_number_sint64(object);
}
tb_uint64_t tb_number_uint64(tb_object_t* object)
{
	// check
	tb_number_t* number = tb_number_cast(object);
	tb_assert_and_check_return_val(number, 0);

	// uint64
	switch (number->type)
	{
	case TB_NUMBER_TYPE_UINT64:
		return number->v.u64;
	case TB_NUMBER_TYPE_SINT64:
		return number->v.s64;
	case TB_NUMBER_TYPE_UINT32:
		return number->v.u32;
	case TB_NUMBER_TYPE_SINT32:
		return number->v.s32;
	case TB_NUMBER_TYPE_UINT16:
		return number->v.u16;
	case TB_NUMBER_TYPE_SINT16:
		return number->v.s16;
	case TB_NUMBER_TYPE_UINT8:
		return number->v.u8;
	case TB_NUMBER_TYPE_SINT8:
		return number->v.s8;
	case TB_NUMBER_TYPE_FLOAT:
		return (tb_uint64_t)number->v.f;
	case TB_NUMBER_TYPE_DOUBLE:
		return (tb_uint64_t)number->v.d;
	default:
		break;
	}

	tb_assert(0);
	return 0;
}

tb_sint64_t tb_number_sint64(tb_object_t* object)
{
	// check
	tb_number_t* number = tb_number_cast(object);
	tb_assert_and_check_return_val(number, 0);

	// sint64
	switch (number->type)
	{
	case TB_NUMBER_TYPE_UINT64:
		return number->v.u64;
	case TB_NUMBER_TYPE_SINT64:
		return number->v.s64;
	case TB_NUMBER_TYPE_UINT32:
		return number->v.u32;
	case TB_NUMBER_TYPE_SINT32:
		return number->v.s32;
	case TB_NUMBER_TYPE_UINT16:
		return number->v.u16;
	case TB_NUMBER_TYPE_SINT16:
		return number->v.s16;
	case TB_NUMBER_TYPE_UINT8:
		return number->v.u8;
	case TB_NUMBER_TYPE_SINT8:
		return number->v.s8;
	case TB_NUMBER_TYPE_FLOAT:
		return (tb_sint64_t)number->v.f;
	case TB_NUMBER_TYPE_DOUBLE:
		return (tb_sint64_t)number->v.d;
	default:
		break;
	}

	tb_assert(0);
	return 0;
}
tb_float_t tb_number_float(tb_object_t* object)
{
	// check
	tb_number_t* number = tb_number_cast(object);
	tb_assert_and_check_return_val(number, 0);

	// float
	switch (number->type)
	{
	case TB_NUMBER_TYPE_FLOAT:
		return number->v.f;
	case TB_NUMBER_TYPE_DOUBLE:
		return (tb_float_t)number->v.d;
	case TB_NUMBER_TYPE_UINT8:
		return (tb_float_t)number->v.u8;
	case TB_NUMBER_TYPE_SINT8:
		return (tb_float_t)number->v.s8;
	case TB_NUMBER_TYPE_UINT16:
		return (tb_float_t)number->v.u16;
	case TB_NUMBER_TYPE_SINT16:
		return (tb_float_t)number->v.s16;
	case TB_NUMBER_TYPE_UINT32:
		return (tb_float_t)number->v.u32;
	case TB_NUMBER_TYPE_SINT32:
		return (tb_float_t)number->v.s32;
	case TB_NUMBER_TYPE_UINT64:
		return (tb_float_t)number->v.u64;
	case TB_NUMBER_TYPE_SINT64:
		return (tb_float_t)number->v.s64;
	default:
		break;
	}

	tb_assert(0);
	return 0;
}
tb_double_t tb_number_double(tb_object_t* object)
{
	// check
	tb_number_t* number = tb_number_cast(object);
	tb_assert_and_check_return_val(number, 0);

	// double
	switch (number->type)
	{
	case TB_NUMBER_TYPE_DOUBLE:
		return number->v.d;
	case TB_NUMBER_TYPE_FLOAT:
		return (tb_double_t)number->v.f;
	case TB_NUMBER_TYPE_UINT8:
		return (tb_double_t)number->v.u8;
	case TB_NUMBER_TYPE_SINT8:
		return (tb_double_t)number->v.s8;
	case TB_NUMBER_TYPE_UINT16:
		return (tb_double_t)number->v.u16;
	case TB_NUMBER_TYPE_SINT16:
		return (tb_double_t)number->v.s16;
	case TB_NUMBER_TYPE_UINT32:
		return (tb_double_t)number->v.u32;
	case TB_NUMBER_TYPE_SINT32:
		return (tb_double_t)number->v.s32;
	case TB_NUMBER_TYPE_UINT64:
		return (tb_double_t)number->v.u64;
	case TB_NUMBER_TYPE_SINT64:
		return (tb_double_t)number->v.s64;
	default:
		break;
	}

	tb_assert(0);
	return 0;
}

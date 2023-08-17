/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual 
* and technical concepts contained herein are proprietary to  Adobe Systems 
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign 
* Patents,patents in process,and are protected by trade secret or copyright 
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from 
* Adobe Systems Incorporated.
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _PointRect_h
#define _PointRect_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

/// A point contains x and y as floating-point values.

class SC_API Point : public Heap
{
public:
	/// The X coordinate or the width.
	double	x;
	/// The Y coordinate or the height.
	double	y;
	/// The default ctor sets the pair to (0,0);
	Point () : Heap(), x(0), y(0) {}
	/// A ctor with two values.
	Point (double xx, double yy) : Heap(), x(xx), y(yy) {}
	/// The copy ctor.
	Point (const Point& r) : Heap(), x(r.x), y(r.y) {}
	/// The assignment operator.
	Point& operator= (const Point& r ) { x = r.x; y = r.y; return *this; }
	/// The equality operator.
	bool   operator== (const Point& r) const { return (x == r.x && y == r.y); }
	/// The non-equality operator.
	bool   operator!= (const Point& r) const { return (x != r.x || y != r.y); }
	/// Comparison: return 1 if this point is to the left and below r, 0 if equal, -1 if right or above.
	int		cmp (const Point& r) const;
};

/// A rectangle contains the left, top, right and bottom values as floating-point values.

class SC_API Rect : public Heap
{
public:
	/// The left border.
	double	left;
	/// The top border.
	double	top;
	/// The right border.
	double	right;
	/// The bottom border.
	double	bottom;
	/// The default ctor sets up an empty Rect at (0,0);
							Rect();
	/// A ctor with four values for left, top, right and bottom.
							Rect (double left, double top, double right, double bottom);
	/// A ctor with the top left and bottom right coordinates.
							Rect (const Point& topLeft, const Point& bottomRight);
	/// The copy ctor.
							Rect (const Rect& r);
	/// The assignment operator.
			Rect&			operator= (const Rect& r );
	/// The equality operator.
			bool			operator== (const Rect& r) const;
	/// The non-equality operator.
	inline	bool			operator!= (const Rect& r) const { return !(*this == r); }
	/// Comparison: compare toplefts; if equal, compare bottomrights. Use Point comparison rules.
			int				cmp (const Rect& r) const;
	/**
	Test whether the Rect is empty. This is the case if either right < left
	or bottom < top.
	@return					true if so
	*/
	inline	bool			isEmpty() const	{ return bool (right < left || bottom < top); }
	/**
	Move the Rect to an absolute position.
	@param	x				the x coordinate
	@param	y				the y coordinate
	*/
			void			moveTo (double x, double y);
	///
	inline	void			moveTo (const Point& r) { moveTo (r.x, r.y); }
	/**
	Move the Rect to a relative position.
	@param	x				the x coordinate
	@param	y				the y coordinate
	*/
			void			moveBy (double x, double y);
	///
	inline	void			moveBy (const Point& r) { moveBy (r.x, r.y); }
	/**
	Inset the Rect.
	@param	dx				the number of horizontal units
	@param	dy				the number of vertical units
			void			inset (double dx, double dy);
	*/
			void			inset (double x, double y);
	///
	inline	void			inset (const Point& r) { inset (r.x, r.y); }
	/**
	Grow or shrink a Rect.
	@param	dx				the number of horizontal units
	@param	dy				the number of vertical units
	*/
	inline	void			grow (double dx, double dy) { right += dx; bottom += dy; }
	///
	inline	void			grow (const Point& r) { right += r.x; bottom += r.y; }
	/**
	Check for a point being inside a Rect.
	@param	x				the x coordinate
	@param	y				the y coordinate
	@return					true if so
	*/
			bool			isInside (double x, double y) const;
	///
	inline	bool			isInside (const Point& r) const { return isInside (r.x, r.y); }
	/**
	Check for a Rect being completely inside a Rect.
	@param	r				the Rect
	@return					true if so
	*/
			bool			isInside (const Rect& r) const;
	/**
	Intersect two rectangles. The "this" instance receives the result of the operation.
	@param	r1				the first Rect
	@param	r2				the second Rect
	@return					true if the resulting Rect is non-empty
	*/
			bool			intersect (const Rect& r1, const Rect& r2);
	/**
	combine two rectangles. The "this" instance receives the result of the operation.
	@param	r1				the first Rect
	@param	r2				the second Rect
	@return					true if the resulting Rect is non-empty
	*/
			bool			combine (const Rect& r1, const Rect& r2);
};

}	// end Namespace

/// Convenience typedefs for Macintosh computers
typedef ScCore::Point ScPoint;
///
typedef ScCore::Rect  ScRect;

#include "ScPostConfig.h"

#endif


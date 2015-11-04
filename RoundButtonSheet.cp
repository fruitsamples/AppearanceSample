/*
	File:		RoundButtonSheet.cp

	Contains:	Sheet to create a chasing arrows control.

	Version:	Mac OS X

	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
				("Apple") in consideration of your agreement to the following terms, and your
				use, installation, modification or redistribution of this Apple software
				constitutes acceptance of these terms.  If you do not agree with these terms,
				please do not use, install, modify or redistribute this Apple software.

				In consideration of your agreement to abide by the following terms, and subject
				to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
				copyrights in this original Apple software (the "Apple Software"), to use,
				reproduce, modify and redistribute the Apple Software, with or without
				modifications, in source and/or binary forms; provided that if you redistribute
				the Apple Software in its entirety and without modifications, you must retain
				this notice and the following text and disclaimers in all such redistributions of
				the Apple Software.  Neither the name, trademarks, service marks or logos of
				Apple Computer, Inc. may be used to endorse or promote products derived from the
				Apple Software without specific prior written permission from Apple.  Except as
				expressly stated in this notice, no other rights or licenses, express or implied,
				are granted by Apple herein, including but not limited to any patent rights that
				may be infringed by your derivative works or by other works in which the Apple
				Software may be incorporated.

				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
				WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
				WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
				PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
				COMBINATION WITH YOUR PRODUCTS.

				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
				CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
				GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
				ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
				OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
				(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
				ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Copyright © 2000-2001 Apple Computer, Inc., All Rights Reserved
*/

#include "RoundButtonSheet.h"
#include "AppearanceHelpers.h"
#if !BUILDING_FOR_CARBON_8
	#include <Carbon/Carbon.h>
#else
	#include <Carbon.h>
#endif

const ControlID 	kRoundButtonSize = { 'Size', 0 }, kRoundButtonIcon = { 'Icon', 0 };

OSStatus IconChangedHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
	ControlRef iconPopup, sizePopup;

	::GetControlByID( (WindowRef)inUserData, &kRoundButtonIcon, &iconPopup );
	::GetControlByID( (WindowRef)inUserData, &kRoundButtonSize, &sizePopup );

	//deactivate size popup if using help icon since help icon implies a size
	if (GetControlValue(iconPopup) == 4)
		DeactivateControl(sizePopup);
	else
		ActivateControl(sizePopup);
	
	return noErr;
}

RoundButtonSheet::RoundButtonSheet( TWindow* parent )
		: CDEFTesterSheet( CFSTR( "Round Button" ), parent )
{
	ControlRef control;
	EventTypeSpec myEventTypes = {kEventClassControl,kEventControlHit};

	::GetControlByID( GetWindowRef(), &kRoundButtonIcon, &control );

	InstallControlEventHandler(control, IconChangedHandler, 1, &myEventTypes, GetWindowRef(), nil);
	
	Show();
}

RoundButtonSheet::~RoundButtonSheet()
{
}


ControlRef
RoundButtonSheet::CreateControl()
{
	ControlRef				control;
	Rect					bounds = { 0, 0, 20, 20 };
	ControlSize				controlSize = kControlSizeNormal;
	SInt16					baseLine;
	ControlButtonContentInfo roundButtonContent;
	OSType					whichIcon = 0;
	OSErr					theErr;
	IconRef					theIcon = 0;
	
	::GetControlByID( GetWindowRef(), &kRoundButtonSize, &control );
	switch ( ::GetControlValue( control ) )
	{
		default:
		case 1:
			controlSize = kControlSizeNormal;
			break;

		case 2:
			controlSize = kControlSizeLarge;
			break;
	}
	
	::GetControlByID( GetWindowRef(), &kRoundButtonIcon, &control );
	switch ( ::GetControlValue( control ) )
	{
		case 2:
			whichIcon = kBackwardArrowIcon;
			break;

		case 3:
			whichIcon = kForwardArrowIcon;
			break;

		case 4:
			whichIcon = kHelpIcon;
			break;
		
		default:
			whichIcon = 0;
	}

	if (whichIcon)
	{
		roundButtonContent.contentType = kControlContentIconRef;
		theErr = GetIconRef(kOnSystemDisk, kSystemIconsCreator, whichIcon, &theIcon);
		roundButtonContent.u.iconRef = theIcon;
	}
	else
		roundButtonContent.contentType = kControlNoContent;
	
	verify_noerr( CreateRoundButtonControl( GetParentWindowRef(), &bounds, controlSize, &roundButtonContent, &control ) );

	if ( control )
	{
		SetControlVisibility( control, false, false );

		::SetControlData( control, 0, kControlSizeTag, sizeof( ControlSize ), &controlSize );
		
		if ( GetBestControlRect( control, &bounds, &baseLine ) == noErr )
			SetControlBounds( control, &bounds );
	}
	
	return control;
}

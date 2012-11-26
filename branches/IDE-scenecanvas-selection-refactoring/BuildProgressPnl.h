/** \file
 *  Game Develop
 *  2008-2012 Florian Rival (Florian.Rival@gmail.com)
 */

#ifndef BUILDPROGRESSPNL_H
#define BUILDPROGRESSPNL_H

//(*Headers(BuildProgressPnl)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/gauge.h>
//*)
#include <wx/thread.h>
#include <wx/stopwatch.h>
class Game;
class SceneCanvas;

class BuildProgressPnl: public wxPanel
{
	public:

		BuildProgressPnl(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~BuildProgressPnl();

		//(*Declarations(BuildProgressPnl)
		wxTextCtrl* tasksLogEdit;
		wxStaticText* statusTxt;
		wxGauge* progressGauge;
		//*)

    protected:

		//(*Identifiers(BuildProgressPnl)
		static const long ID_STATICTEXT1;
		static const long ID_GAUGE1;
		static const long ID_TEXTCTRL1;
		//*)

	private:

		//(*Handlers(BuildProgressPnl)
		void OntasksLogEditText(wxCommandEvent& event);
		//*)

		/**
         * Add text to log. Prevent duplicates.
		 */
		void AppendText(wxString text);

		/**
		 * Called thanks to Event of type CodeCompiler::refreshEventType sent ( typically ) by CodeCompiler.
		 */
		void OnMustRefresh(wxCommandEvent&);

		wxString lastTextAdded; ///< Used to prevent duplicates
		wxStopWatch compilationTimer;
		bool clearOnNextTextAdding;

		DECLARE_EVENT_TABLE()
};

#endif


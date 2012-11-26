/** \file
 *  Game Develop
 *  2008-2012 Florian Rival (Florian.Rival@gmail.com)
 */
#include "InitialPositionBrowserDlg.h"

//(*InternalHeaders(InitialPositionBrowserDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "GDL/InitialInstancesContainer.h"
#include "GDL/Position.h"
#include "SceneCanvas.h"

//(*IdInit(InitialPositionBrowserDlg)
const long InitialPositionBrowserDlg::ID_LISTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(InitialPositionBrowserDlg,wxPanel)
	//(*EventTable(InitialPositionBrowserDlg)
	//*)
END_EVENT_TABLE()

InitialPositionBrowserDlg::InitialPositionBrowserDlg(wxWindow* parent, InitialInstancesContainer & initialInstancesContainer_, SceneCanvas & sceneCanvas_) :
instancesContainer(initialInstancesContainer_),
sceneCanvas(sceneCanvas_)
{
	//(*Initialize(InitialPositionBrowserDlg)
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	initialPositionsList = new wxListCtrl(this, ID_LISTCTRL1, wxDefaultPosition, wxSize(206,167), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));
	FlexGridSizer1->Add(initialPositionsList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&InitialPositionBrowserDlg::OninitialPositionsListItemActivated);
	//*)

	initialPositionsList->InsertColumn(0, _("Object"));
	initialPositionsList->InsertColumn(1, _("X"));
	initialPositionsList->InsertColumn(2, _("Y"));
	initialPositionsList->InsertColumn(3, _("Angle"));
	initialPositionsList->InsertColumn(4, _("Z order"));

	Refresh();
}

InitialPositionBrowserDlg::~InitialPositionBrowserDlg()
{
	//(*Destroy(InitialPositionBrowserDlg)
	//*)
}

void InitialPositionBrowserDlg::Refresh()
{
    initialPositionsList->DeleteAllItems();
    for (unsigned int i = 0;i<instancesContainer.GetInstancesCount();++i)
    {
        initialPositionsList->InsertItem(i, instancesContainer.GetInstance(i).GetObjectName());
        initialPositionsList->SetItem(i, 1, ToString(instancesContainer.GetInstance(i).GetX()));
        initialPositionsList->SetItem(i, 2, ToString(instancesContainer.GetInstance(i).GetY()));
        initialPositionsList->SetItem(i, 3, ToString(instancesContainer.GetInstance(i).GetAngle()));
        initialPositionsList->SetItem(i, 4, ToString(instancesContainer.GetInstance(i).GetZOrder()));
    }
}

/**
 * Scroll to object in Scene Editor when double clicking on it
 */
void InitialPositionBrowserDlg::OninitialPositionsListItemActivated(wxListEvent& event)
{
    if ( event.GetIndex() < 0 ) return;

    unsigned long id = static_cast<unsigned long>(event.GetIndex());

    if ( id >= instancesContainer.GetInstancesCount() ) return;

    sceneCanvas.GetEditionView().setCenter( instancesContainer.GetInstance(id).GetX(),  instancesContainer.GetInstance(id).GetY());

    //Update scene canvas selection with list selection
    sceneCanvas.ClearSelection();

    long itemIndex = -1;
    for (;;) { //Iterate over all controls
        itemIndex = initialPositionsList->GetNextItem(itemIndex,
                                             wxLIST_NEXT_ALL,
                                             wxLIST_STATE_SELECTED);

        if (itemIndex == -1) break;

        // Add each selected object to scene canvas selection
        if ( itemIndex < instancesContainer.GetInstancesCount() && itemIndex >= 0 )
        {
            sceneCanvas.SelectObject(sceneCanvas.GetObjectFromInitialPosition(instancesContainer.GetInstance(itemIndex)));
        }
    }
}

void InitialPositionBrowserDlg::DeselectAll()
{
    int size = initialPositionsList->GetItemCount();
    for (int i = 0; i < size; ++i)
        initialPositionsList->SetItemState(i,0,wxLIST_STATE_SELECTED);
}

void InitialPositionBrowserDlg::SelectInitialPosition(const gd::InitialInstance & instance)
{
    for (unsigned int i = 0;i<instancesContainer.GetInstancesCount();++i)
    {
        if ( &instancesContainer.GetInstance(i) == &instance )
        {
            if ( i >= initialPositionsList->GetItemCount() ) return;

            initialPositionsList->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            initialPositionsList->EnsureVisible(i);
            return;
        }
    }
}


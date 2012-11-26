/** \file
 *  Game Develop
 *  2008-2012 Florian Rival (Florian.Rival@gmail.com)
 */
#include "EditorLayers.h"

//(*InternalHeaders(EditorLayers)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)
#include <wx/config.h>
#include <wx/log.h>
#include "GDL/CommonTools.h"
#include "GDL/Position.h"
#include "GDCore/Tools/HelpFileAccess.h"
#include "ObjectsOnBadLayerBox.h"
#include "EditLayer.h"
#include "SceneCanvas.h"
#include "GDCore/IDE/wxTools/SkinHelper.h"
#ifdef __WXGTK__
#include <gtk/gtk.h>
#endif

//(*IdInit(EditorLayers)
const long EditorLayers::ID_AUITOOLBAR1 = wxNewId();
const long EditorLayers::ID_PANEL3 = wxNewId();
const long EditorLayers::ID_LISTCTRL1 = wxNewId();
const long EditorLayers::idMenuEdit = wxNewId();
const long EditorLayers::idMenuAdd = wxNewId();
const long EditorLayers::idMenuDel = wxNewId();
const long EditorLayers::idMenuUp = wxNewId();
const long EditorLayers::idMenuDown = wxNewId();
//*)
const long EditorLayers::ID_BITMAPBUTTON1 = wxNewId();
const long EditorLayers::ID_BITMAPBUTTON6 = wxNewId();
const long EditorLayers::ID_BITMAPBUTTON3 = wxNewId();

BEGIN_EVENT_TABLE(EditorLayers,wxPanel)
	//(*EventTable(EditorLayers)
	//*)
END_EVENT_TABLE()

EditorLayers::EditorLayers(wxWindow* parent, Game & game_, Scene & scene_, gd::MainFrameWrapper & mainFrameWrapper_) :
game(game_),
scene(scene_),
sceneCanvas(NULL),
mainFrameWrapper(mainFrameWrapper_)
{

	//(*Initialize(EditorLayers)
	wxMenuItem* MenuItem5;
	wxMenuItem* MenuItem4;
	wxMenuItem* MenuItem3;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(1);
	toolBarPanel = new wxPanel(this, ID_PANEL3, wxDefaultPosition, wxSize(120,25), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	AuiManager1 = new wxAuiManager(toolBarPanel, wxAUI_MGR_DEFAULT);
	toolbar = new wxAuiToolBar(toolBarPanel, ID_AUITOOLBAR1, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
	toolbar->Realize();
	AuiManager1->AddPane(toolbar, wxAuiPaneInfo().Name(_T("PaneName")).ToolbarPane().Caption(_("Pane caption")).Layer(10).Top().Gripper(false));
	AuiManager1->Update();
	FlexGridSizer1->Add(toolBarPanel, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	layersList = new wxListCtrl(this, ID_LISTCTRL1, wxDefaultPosition, wxSize(191,198), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));
	FlexGridSizer1->Add(layersList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(FlexGridSizer1);
	MenuItem1 = new wxMenuItem((&contextMenu), idMenuEdit, _("Edit the layer properties"), wxEmptyString, wxITEM_NORMAL);
	MenuItem1->SetBitmap(wxBitmap(wxImage(_T("res/editnom.png"))));
	contextMenu.Append(MenuItem1);
	contextMenu.AppendSeparator();
	MenuItem2 = new wxMenuItem((&contextMenu), idMenuAdd, _("Add a layer"), wxEmptyString, wxITEM_NORMAL);
	MenuItem2->SetBitmap(wxBitmap(wxImage(_T("res/addicon.png"))));
	contextMenu.Append(MenuItem2);
	MenuItem3 = new wxMenuItem((&contextMenu), idMenuDel, _("Delete the layer"), wxEmptyString, wxITEM_NORMAL);
	MenuItem3->SetBitmap(wxBitmap(wxImage(_T("res/deleteicon.png"))));
	contextMenu.Append(MenuItem3);
	contextMenu.AppendSeparator();
	MenuItem4 = new wxMenuItem((&contextMenu), idMenuUp, _("Move over"), wxEmptyString, wxITEM_NORMAL);
	MenuItem4->SetBitmap(wxBitmap(wxImage(_T("res/up.png"))));
	contextMenu.Append(MenuItem4);
	MenuItem5 = new wxMenuItem((&contextMenu), idMenuDown, _("Move below"), wxEmptyString, wxITEM_NORMAL);
	MenuItem5->SetBitmap(wxBitmap(wxImage(_T("res/down.png"))));
	contextMenu.Append(MenuItem5);
	imageList = new wxImageList(16, 16, 1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	toolBarPanel->Connect(wxEVT_SIZE,(wxObjectEventFunction)&EditorLayers::OntoolBarPanelResize,0,this);
	Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&EditorLayers::OnlayersListItemSelect1);
	Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&EditorLayers::OnlayersListItemActivated);
	Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_FOCUSED,(wxObjectEventFunction)&EditorLayers::OnlayersListItemFocused);
	Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&EditorLayers::OnlayersListItemRClick);
	Connect(idMenuEdit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnEditSelected1);
	Connect(idMenuAdd,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnAddSelected);
	Connect(idMenuDel,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnDelSelected);
	Connect(idMenuUp,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnUpSelected);
	Connect(idMenuDown,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnDownSelected);
	//*)
	Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnRefresh);
	Connect(ID_BITMAPBUTTON6,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnMoreOptions);
	Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EditorLayers::OnHelp);

    imageList->Add(wxBitmap("res/rightArrowGrey.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/1rightarrow.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/eye.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/eyeGrey.png", wxBITMAP_TYPE_ANY));
    layersList->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

	layersList->InsertColumn(1, _("Layer"));
	layersList->InsertColumn(2, _("Visible"));

    CreateToolbar();
    gd::SkinHelper::ApplyCurrentSkin(*toolbar);

    Refresh();
}

////////////////////////////////////////////////////////////
/// Mise � jour de la barre d'outils
////////////////////////////////////////////////////////////
void EditorLayers::CreateToolbar()
{
    toolbar->SetToolBitmapSize( wxSize( 16, 16 ) );
    toolbar->AddTool( ID_BITMAPBUTTON1, _( "Refresh" ), wxBitmap( wxImage( "res/refreshicon.png" ) ), _("Refresh the layer list") );
    toolbar->AddSeparator();
    toolbar->AddTool( idMenuAdd, _( "Add a layer" ), wxBitmap( wxImage( "res/addicon.png" ) ), _("Add a layer") );
    toolbar->AddTool( idMenuDel, _( "Delete the selected layer" ), wxBitmap( wxImage( "res/deleteicon.png" ) ), _("Delete the selected layer") );
    toolbar->AddSeparator();
    toolbar->AddTool( idMenuUp, _( "Move the layer over" ), wxBitmap( wxImage( "res/up.png" ) ), _("Move the layer over") );
    toolbar->AddTool( idMenuDown, _( "Move the layer below" ), wxBitmap( wxImage( "res/down.png" ) ), _("Move the layer below") );
    toolbar->AddTool( ID_BITMAPBUTTON6, _( "More edition options ( right click on the list )" ), wxBitmap( wxImage( "res/moreicon.png" ) ), _("More edition options ( right click on the list )") );
    toolbar->AddSeparator();
    toolbar->AddTool( ID_BITMAPBUTTON3, _( "Objects' editor help" ), wxBitmap( wxImage( "res/helpicon.png" ) ), _("Objects' editor help") );
    toolbar->Realize();
}

////////////////////////////////////////////////////////////
/// Redimensionnement de la toolbar
////////////////////////////////////////////////////////////
void EditorLayers::OntoolBarPanelResize(wxSizeEvent& event)
{
    toolbar->SetSize(toolBarPanel->GetSize().x, -1);
}

EditorLayers::~EditorLayers()
{
	//(*Destroy(EditorLayers)
	//*)
	AuiManager1->UnInit();
}

void EditorLayers::OnRefresh(wxCommandEvent& event)
{
    Refresh();
}

void EditorLayers::OnMoreOptions(wxCommandEvent& event)
{
    PopupMenu(&contextMenu);
}

void EditorLayers::OnHelp(wxCommandEvent& event)
{
    gd::HelpFileAccess::GetInstance()->OpenURL(_("http://www.wiki.compilgames.net/doku.php/en/game_develop/documentation/manual/editors/scene_editor/edit_layer"));
}

////////////////////////////////////////////////////////////
/// Rafraichissement de la liste
////////////////////////////////////////////////////////////
void EditorLayers::Refresh()
{
    layersList->DeleteAllItems();

    for (unsigned int i =0;i<scene.GetLayersCount();++i)
    {
        string name = scene.GetLayer(i).GetName();
        if ( name == "" ) name = _("Base layer");
    	layersList->InsertItem(0, name);

    	if ( scene.GetLayer(i).GetVisibility() )
            layersList->SetItemColumnImage(0, 1, 2);
        else
            layersList->SetItemColumnImage(0, 1, 3);

        layersList->SetItemImage(0,-1,0);
    }
    layersList->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    layersList->SetColumnWidth( 1, wxLIST_AUTOSIZE );

    UpdateSelectedLayerIcon();
}

void EditorLayers::UpdateSelectedLayerIcon()
{
    if ( !sceneCanvas ) return;

    for (unsigned int i =0;i<scene.GetLayersCount();++i)
    {
    	if ( scene.GetLayer(i).GetName() == sceneCanvas->GetCurrentLayer() )
            layersList->SetItemImage(scene.GetLayersCount()-i-1,1,1);
        else
            layersList->SetItemImage(scene.GetLayersCount()-i-1,-1,-1);
    }
}

////////////////////////////////////////////////////////////
/// Ajouter un calque
////////////////////////////////////////////////////////////
void EditorLayers::OnAddSelected(wxCommandEvent& event)
{
    wxString name = _("New layer");

    bool alreadyExist = false;
    int nb = 0;
    for (unsigned int i = 0;i<scene.GetLayersCount();++i)
    {
    	if ( scene.GetLayer(i).GetName() == name )
            alreadyExist = true;
    }
    while ( alreadyExist )
    {
        ++nb;
        name = _("New layer ") + ToString(nb);

        alreadyExist = false;
        for (unsigned int i = 0;i<scene.GetLayersCount();++i)
        {
            if ( scene.GetLayer(i).GetName() == name )
                alreadyExist = true;
        }
    }

    Layer layer;
    layer.SetName(static_cast<string>(name));
    layer.SetCameraCount(1);
    scene.InsertLayer(layer, scene.GetLayersCount()-1);

    Refresh();
    if ( sceneCanvas ) sceneCanvas->Reload();
}

/**
 * Delete a layer
 */
void EditorLayers::OnDelSelected(wxCommandEvent& event)
{
    //Get selected layer
    Layer * selectedLayer = GetSelectedLayer();
    if ( !selectedLayer || selectedLayer->GetName().empty() ) return;

    std::string name = selectedLayer->GetName();

    for (unsigned int i = 0;i<scene.GetLayersCount();++i)
    {
    	if ( &scene.GetLayer(i) == selectedLayer )
    	{
            //Verifying if there are objects on this layer
            bool objectsOnThisLayer = false;
            for (unsigned int j =0;j<scene.GetInitialInstances().GetInstancesCount();++j)
            {
                if ( scene.GetInitialInstances().GetInstance(j).GetLayer() == name )
                    objectsOnThisLayer = true;
            }

    	    //Confirmation de la suppression et choix du sort des objets dessus
    	    if ( objectsOnThisLayer )
    	    {
    	        std::vector<std::string> availableLayers;
    	        for (unsigned int j = 0;j<scene.GetLayersCount();++j)
    	        {
    	            if (i!=j) availableLayers.push_back(scene.GetLayer(j).GetName());
    	        }

                ObjectsOnBadLayerBox dialog(this, availableLayers);
                int choice = dialog.ShowModal();

                if ( choice == 0 ) return; //Annulation
                else if ( choice == 1 )
                    scene.GetInitialInstances().RemoveAllInstancesOnLayer(name);
                else if ( choice == 2 )
                    scene.GetInitialInstances().MoveInstancesToLayer(name, dialog.moveOnLayerNamed);
    	    }

            //Delete the layer and select base layer
    	    scene.RemoveLayer(name);
            if ( sceneCanvas )
            {
                sceneCanvas->SetCurrentLayer("");
                sceneCanvas->Reload();
            }
            Refresh();
    	    return;
    	}
    }
    wxLogWarning(_("Can't find the layer to delete !"));
}

////////////////////////////////////////////////////////////
/// Remonter un calquer
////////////////////////////////////////////////////////////
void EditorLayers::OnUpSelected(wxCommandEvent& event)
{
    //Get selected layer
    Layer * selectedLayer = GetSelectedLayer();
    if ( !selectedLayer ) return;

    for (unsigned int i = 0;i<scene.GetLayersCount();++i)
    {
    	if ( &scene.GetLayer(i) == selectedLayer )
    	{
    	    if ( i <= scene.GetLayersCount()-1-1 )
    	    {
    	        scene.SwapLayers(i,i+1);

                Refresh();
                if ( sceneCanvas ) sceneCanvas->Reload();

                //On reslectionne le calque
                layersList->SetItemState(scene.GetLayersCount()-i-1-1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    	    }
    	    return;
    	}
    }
    wxLogWarning(_("Can't find the layer to move  !"));
}

////////////////////////////////////////////////////////////
/// Descendre un calque
////////////////////////////////////////////////////////////
void EditorLayers::OnDownSelected(wxCommandEvent& event)
{
    //Get selected layer
    Layer * selectedLayer = GetSelectedLayer();
    if ( !selectedLayer ) return;

    for (unsigned int i = 0;i<scene.GetLayersCount();++i)
    {
    	if ( &scene.GetLayer(i) == selectedLayer )
    	{
    	    if ( i >= 1 )
    	    {
    	        //On d�place le calque
    	        scene.SwapLayers(i,i-1);

                Refresh();
                if ( sceneCanvas ) sceneCanvas->Reload();

                //On reslectionne le calque
                layersList->SetItemState(scene.GetLayersCount()-i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    	    }
    	    return;
    	}
    }
    wxLogWarning(_("Can't find the layer to move  !"));
}

////////////////////////////////////////////////////////////
/// Clic droit : Mise � jour du calque selectionn� et menu contextuel
////////////////////////////////////////////////////////////
void EditorLayers::OnlayersListItemRClick(wxListEvent& event)
{
    PopupMenu(&contextMenu);
}

////////////////////////////////////////////////////////////
/// Connaitre le num�ro de l'item selectionn� dans la liste
////////////////////////////////////////////////////////////
Layer* EditorLayers::GetSelectedLayer()
{
    long itemIndex = -1;

    for (;;)
    {
        itemIndex = layersList->GetNextItem(itemIndex,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
        if (itemIndex == -1) break;

        // Got the selected item index
        unsigned int layerId = scene.GetLayersCount()-itemIndex-1;
        if ( layerId < scene.GetLayersCount() )
        {
            return &scene.GetLayer(layerId);
        }
    }

    return NULL;
}

////////////////////////////////////////////////////////////
/// Selection d'un item : Mise � jour du calque selectionn�
////////////////////////////////////////////////////////////
void EditorLayers::OnlayersListItemActivated(wxListEvent& event)
{
    //Get selected layer
    Layer * selectedLayer = GetSelectedLayer();
    if ( !selectedLayer ) return;

    //Get selected column
    wxPoint click_point=::wxGetMousePosition();
    wxPoint list_point=layersList->GetScreenPosition();

    // delta x
    int dx=click_point.x - list_point.x;

    // work out the column
    int ex=0; // cumulative sum of column widths
    int column = -1;
    for (column=0; column<layersList->GetColumnCount(); column++) {
            ex+=layersList->GetColumnWidth(column);
            if (ex > dx) break;
    }

    if ( column == 1 )
    {
        selectedLayer->SetVisibility(!selectedLayer->GetVisibility());
        Refresh();

        //Changes without reloading
        if ( sceneCanvas )
        {
            for (unsigned int i = 0;i<sceneCanvas->GetRuntimeScene().GetLayersCount();++i)
            {
                if ( sceneCanvas->GetRuntimeScene().GetLayer(i).GetName() == selectedLayer->GetName() )
                    sceneCanvas->GetRuntimeScene().GetLayer(i).SetVisibility(selectedLayer->GetVisibility());
            }
        }
        return;
    }
    else
    {
        EditSelectedLayer();
    }
}

void EditorLayers::EditSelectedLayer()
{
    //Get selected layer
    Layer * layer = GetSelectedLayer();
    if ( !layer ) return;

    //Edition du calque
    string oldName = layer->GetName();
    EditLayer dialog(this, *layer);
    dialog.ShowModal();

    //On renomme les objets qui sont sur le calque
    if ( layer->GetName() != oldName )
    {
        scene.GetInitialInstances().MoveInstancesToLayer(oldName, layer->GetName());
        if ( sceneCanvas && sceneCanvas->GetCurrentLayer() == oldName ) sceneCanvas->SetCurrentLayer(layer->GetName());
    }

    Refresh();
    if ( sceneCanvas ) sceneCanvas->Reload();
}

////////////////////////////////////////////////////////////
/// Clic sur le bouton d'�dition
////////////////////////////////////////////////////////////
void EditorLayers::OnEditSelected1(wxCommandEvent& event)
{
    EditSelectedLayer();
}

////////////////////////////////////////////////////////////
/// Selection d'un item :Mise � jour du calque selectionn�
////////////////////////////////////////////////////////////
void EditorLayers::OnlayersListItemSelect1(wxListEvent& event)
{
    //Get selected layer
    Layer * layer = GetSelectedLayer();
    if ( !layer ) return;

    if ( sceneCanvas ) sceneCanvas->SetCurrentLayer(layer->GetName());
    UpdateSelectedLayerIcon();
}

void EditorLayers::OnlayersListItemFocused(wxListEvent& event)
{
    //Get selected layer
    Layer * layer = GetSelectedLayer();
    if ( !layer ) return;

    if ( sceneCanvas ) sceneCanvas->SetCurrentLayer(layer->GetName());
    UpdateSelectedLayerIcon();
}


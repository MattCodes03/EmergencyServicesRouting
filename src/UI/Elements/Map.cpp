#include "Map.h"

void Map::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);

    // Load the PNG image
    wxBitmap pngBitmap("Assets/map.png", wxBITMAP_TYPE_PNG);

    if (!pngBitmap.IsOk())
    {
        wxLogError("Failed to load PNG image");
        return;
    }

    // Draw the PNG image on the DC
    dc.DrawBitmap(pngBitmap, 0, 0, true);

    // DrawGrid(dc);
};

void Map::DrawGrid(wxDC &dc)
{
    // Get the size of the canvas
    wxSize size = GetClientSize();

    // Define the number of rows and columns for your grid
    int numRows = 10;
    int numCols = 10;

    // Calculate cell size
    int cellWidth = size.x / numCols;
    int cellHeight = size.y / numRows;

    // Draw the grid
    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols; ++col)
        {
            int x = col * cellWidth;
            int y = row * cellHeight;

            // Draw a rectangle for each cell
            dc.DrawRectangle(x, y, cellWidth, cellHeight);

            // You can add more drawing logic based on your map data here
        }
    }
};

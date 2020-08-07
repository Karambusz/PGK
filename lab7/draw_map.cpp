#include <wx/wx.h>
#include "GUIMyFrame1.h"
#include <math.h>


const float limitMin = -2.5;
const float limitMax = 2.5;
// UWAGA: TO JEST JEDYNY PLIK, KTORY NALEZY EDYTOWAC **************************


float shepard(float d[100][3], float x, float y, int N) {
	float licznik = 0.0;
	float mianownik = 0.0;
	float tmp = 0.0;

	for (int i = 0; i < N; ++i) {
		tmp = 1 / fabs((pow(x - d[i][0], 2) + pow(y - d[i][1], 2)));
		licznik += tmp * d[i][2];
		mianownik += tmp;
	}
	return licznik / mianownik;
}


void GUIMyFrame1::DrawMap(int N, float d[100][3], bool Contour, int MappingType, int NoLevels, bool ShowPoints)
{
	wxMemoryDC memDC;
	memDC.SelectObject(MemoryBitmap);
	memDC.SetBackground(*wxWHITE_BRUSH);
	memDC.Clear();

	// demo.....
	memDC.SetPen(*wxBLACK_PEN);
	memDC.SetBrush(*wxTRANSPARENT_BRUSH);

	float points[500][500];
	float pointMax = 0.0;
	float pointMin = 0.0;

	float step = (limitMax - limitMin) / 500.0;

	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < 500; j++) {
			points[i][j] = shepard(d, limitMin + i * step, limitMax - j * step, N);
			if (points[i][j] > pointMax)
				pointMax = points[i][j];
			if (points[i][j] < pointMin)
				pointMin = points[i][j];
		}
	}


	unsigned char r, g, b;

	if (MappingType) {
		for (int i = 0; i < 500; i++) {
			for (int j = 0; j < 500; j++) {
				if (MappingType == 1) {
					float color = ((points[i][j] - pointMin) / (pointMax - pointMin));
					r = 255.0 - color * 255.0;
					g = 0.0;
					b = color * 255.0;
					memDC.SetPen(wxColour(r, g, b));
					memDC.DrawPoint(i, j);
				}
				if (MappingType == 2) {
					float color = ((points[i][j] - pointMin) / (pointMax - pointMin) - 0.5) * 2.0;
					if (color < 0) { r = 255 * fabs(color); }
					else { r = 0; }
					g = 255 - 255 * fabs(color);
					if (color > 0) { b = 255 * color; }
					else { b = 0; }
					memDC.SetPen(wxColour(r, g, b));
					memDC.DrawPoint(i, j);
				}
				if (MappingType == 3) {
					float color = ((points[i][j] - pointMin) / (pointMax - pointMin) - 0.5) * 2.0;
					r = 127 + 127 * color;
					g = 127 + 127 * color;
					b = 127 + 127 * color;
					memDC.SetPen(wxColour(r, g, b));
					memDC.DrawPoint(i, j);
				}

			}
		}
	}

	if (Contour) {
		memDC.SetPen(*wxBLACK_PEN);
		memDC.SetBrush(*wxTRANSPARENT_BRUSH);
		for (int l = 0; l < NoLevels; l++)
		{
			float point = pointMin + (l + 1) * (pointMax - pointMin) / (NoLevels + 1.0);

			for (int i = 0; i < 500; i++) {
				for (int j = 0; j < 500; j++) {
					if (points[j][i] > point)
					{
						for (int k = -1; k <= 1; k++) {
							for (int m = -1; m <= 1; m++) {
								if (k && m && (k + j >= 0) && (m + j >= 0) && (k + j < 500) && (m + i < 500) && (points[k + j][m + i] < point))
									memDC.DrawPoint(j, i);

							}
						}
					}
				}
			}
		}
	} 

	int pointx, pointy;
	if (ShowPoints) {
		for (int i = 0; i < N; i++) {
			pointx = (d[i][0] + 2.5) * 100;
			pointy = (d[i][1] - 2.5) * -100;
			memDC.DrawLine(pointx - 2, pointy, pointx + 3, pointy);
			memDC.DrawLine(pointx, pointy - 2, pointx, pointy + 3);
		}
	}
} 



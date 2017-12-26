// kingimageView.cpp : implementation of the CKingimageView class
//

#include "stdafx.h"
#include "kingimage.h"

#include "kingimageDoc.h"
#include "kingimageView.h"
#include <ctgmath>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKingimageView

IMPLEMENT_DYNCREATE(CKingimageView, CScrollView)

BEGIN_MESSAGE_MAP(CKingimageView, CScrollView)
	//{{AFX_MSG_MAP(CKingimageView)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_PROCESS, OnProcess)
	ON_COMMAND(ID_any, Onany)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_SOBEL, &CKingimageView::OnSobel)
	ON_COMMAND(ID_UNSHARPMASKING, &CKingimageView::OnUnsharpmasking)
	ON_COMMAND(ID_LOGMASK4, &CKingimageView::OnLogmask4)
	ON_COMMAND(ID_LOGMASK3, &CKingimageView::OnLogmask3)
	ON_COMMAND(ID_LOGMASK, &CKingimageView::OnLogmask)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKingimageView construction/destruction

CKingimageView::CKingimageView()
{
	// TODO: add construction code here

}

CKingimageView::~CKingimageView()
{
}

BOOL CKingimageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView drawing

void CKingimageView::OnDraw(CDC* pDC)
{
	CKingimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
//	pDC->SetStretchBltMode(COLORONCOLOR);
//	int x,y,bytes;
	if (pDoc->imagetype==PCX)
		(pDoc->_pcx)->draw(pDC);
	if (pDoc->imagetype==BMP)
		(pDoc->_bmp)->draw(pDC);
	if (pDoc->imagetype==GIF)
		(pDoc->_gif)->draw(pDC);
	if (pDoc->imagetype==JPG)
		(pDoc->_jpg)->draw(pDC);

}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView printing

BOOL CKingimageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKingimageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKingimageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView diagnostics

#ifdef _DEBUG
void CKingimageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CKingimageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CKingimageDoc* CKingimageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKingimageDoc)));
	return (CKingimageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKingimageView message handlers



void CKingimageView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize totalSize=CSize(::GetSystemMetrics(SM_CXSCREEN),
		                  ::GetSystemMetrics(SM_CYSCREEN));
	CSize pageSize=CSize(totalSize.cx/2,totalSize.cy/2);
	CSize lineSize=CSize(totalSize.cx/10,totalSize.cy/10);

	SetScrollSizes(MM_TEXT,totalSize,pageSize,lineSize);
}

void CKingimageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CScrollView::OnMouseMove(nFlags, point);
}


//******************************************************
void CKingimageView::OnProcess()
{
	// TODO: Add your command handler code here
	CKingimageDoc* pDoc = GetDocument();


	int iBitPerPixel = pDoc->_bmp->bitsperpixel;
	int iWidth = pDoc->_bmp->width;
	int iHeight = pDoc->_bmp->height;
	BYTE *pImg = pDoc->_bmp->point;

	


	int Wp = iWidth;
	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		int r = iWidth % 4;
		int p = (4-r)%4;
		Wp = iWidth + p;
	}
	else if (iBitPerPixel == 24)	// RGB image
	{
		int r = (3*iWidth) % 4;
		int p = (4-r)%4;
		Wp = 3*iWidth + p;
	}

	

	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		
	}


	else if(iBitPerPixel == 24)  ////True color 24bits image
	{
		for(int i=0; i<iHeight; i++)
		for(int j=0; j<iWidth; j++)
		{
			pImg[i*Wp+j*3]   = 0;      //B
			// pImg[i*Wp+j*3+1] = 0;      //G
			pImg[i*Wp+j*3+2] = 0;      //R 
		}
	}

	////redraw the screen
	OnDraw(GetDC());
}

void CKingimageView::Onany() 
{
	CKingimageDoc* pDoc = GetDocument();


	int iBitPerPixel = pDoc->_bmp->bitsperpixel;
	int iWidth = pDoc->_bmp->width;
	int iHeight = pDoc->_bmp->height;
	BYTE *pImg = pDoc->_bmp->point;




	int Wp = iWidth;
	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		int r = iWidth % 4;
		int p = (4 - r) % 4;
		Wp = iWidth + p;
	}
	else if (iBitPerPixel == 24)	// RGB image
	{
		int r = (3 * iWidth) % 4;
		int p = (4 - r) % 4;
		Wp = 3 * iWidth + p;
	}

	std::ofstream ofs1;
	ofs1.open("Mask1.txt", std::ofstream::out | std::ofstream::app);

	int i, j,m,n;
	double Mask1[7][7] = {};
	double Mask[7][7] = {};
	double *LoG = new double[Wp*iHeight];
	
	double sigma2 = 1.4*1.4;
	double sigma4 = 1.4*1.4*1.4*1.4;
	int add = 0;
	double con = 2 * 3.14*sigma4;
	int final1[7][7] = {0};
	int newfinal[7][7] = {};
	// ofs1 << "Constant is" << con <<"\n";

	for (int x = 0; x < iWidth; x++)
		for (int y = 0; y < iHeight; y++)

		{
			LoG[x*iWidth + y] = 0;
		}

	for (i = -3,m=0; i <=3,m<7; i++,m++)
	{
		for (j = -3,n=0; j <=3,n<7; j++,n++)
		{
			double a = (i*i + j*j - 2 * sigma2);
			double b = exp(-(i*i + j*j) / (2 * sigma2));
			double final = ((a*b) / con) * 400;
			final1[m][n] = trunc(final);
			newfinal[m][n] = final1[m][n];
			//int add = add + final1;
			ofs1 << final1[m][n] << "\t";

			//Mask1[i][j] =trunc((i*i + j*j - 2 * sigma2) * ( exp(-(i*i + j*j) / (2 * sigma2)))   );
			//Mask1[i][j] =(float) ((i*i + j*j - 2 * sigma2) / (2 * 3.14*sigma4))* exp(-(i*i + j*j) / (2.0 * sigma2));
			//Mask[i][j] = Mask1[i][j];
			//ofs1 << Mask1[i][j] << "\t";
		}
		ofs1 << "\n";


		
	}
	ofs1 << "\n";
	
	for (i = 0; i <7; i++)
	{
		for (j = 0; j <7; j++)
		{
			ofs1 << newfinal[i][j] << "\t";

		}
		ofs1 << "\n";
	}


	for (int x = 0; x < iWidth; x++)
		for (int y = 0; y < iHeight; y++)

		{ 
			
			for (int filterY = 0; filterY < 7; filterY++)
				for (int filterX = 0; filterX < 7; filterX++)
				{
					int imageX = (x - 7 / 2 + filterX + iWidth) % iWidth;
					int imageY = (y - 7 / 2 + filterY + iHeight) % iHeight;
					LoG[imageY * iWidth + imageX] = LoG[imageY * iWidth + imageX] + (final1[filterY][filterX] * pImg[imageY * iWidth + imageX])/49;
					if (LoG[imageY * iWidth + imageX] < 0)
						LoG[imageY * iWidth + imageX] = 0;
					else if (LoG[imageY * iWidth + imageX] >255)
						LoG[imageY * iWidth + imageX] = 255;
				}


		}



	for (int i = 3; i<iHeight - 3; i++)
		for (int j = 3; j < iWidth - 3; j++)
		{
			pImg[i*Wp + j] = LoG[i*Wp + j];
		}

		OnDraw(GetDC());
}

void CKingimageView::OnLogmask4()
{
	// 7x7 MASK

	CKingimageDoc* pDoc = GetDocument();

	int iBitPerPixel = pDoc->_bmp->bitsperpixel;
	int iWidth = pDoc->_bmp->width;
	int iHeight = pDoc->_bmp->height;
	BYTE *pImg = pDoc->_bmp->point;

	int Wp = iWidth;
	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		int r = iWidth % 4;
		int p = (4 - r) % 4;
		Wp = iWidth + p;
	}
	else if (iBitPerPixel == 24)	// RGB image
	{
		int r = (3 * iWidth) % 4;
		int p = (4 - r) % 4;
		Wp = 3 * iWidth + p;
	}

	std::ofstream ofs1;
	ofs1.open("Mask1.txt", std::ofstream::out | std::ofstream::app);

	int i, j, m, n;
	double *LoG = new double[Wp*iHeight];
	double sigma2 = 1.4*1.4;
	double sigma4 = 1.4*1.4*1.4*1.4;
	double con = 2 * 3.14*sigma4;
	int final1[7][7] = { 0 };
	int newfinal[7][7] = {};

	for (int x = 0; x < iWidth; x++)
		for (int y = 0; y < iHeight; y++)
		{
			LoG[x*iWidth + y] = 0;
		}

	for (i = -3, m = 0; i <= 3, m<7; i++, m++)
	{
		for (j = -3, n = 0; j <= 3, n<7; j++, n++)
		{
			double a = (i*i + j*j - 2 * sigma2);
			double b = exp(-(i*i + j*j) / (2 * sigma2));
			double final = ((a*b) / con) * 400;
			final1[m][n] = trunc(final);

			ofs1 << final1[m][n] << "\t";

		}
		ofs1 << "\n";

	}


	for (int x = 0; x < iWidth; x++)
		for (int y = 0; y < iHeight; y++)

		{

			for (int filterY = 0; filterY < 7; filterY++)
				for (int filterX = 0; filterX < 7; filterX++)
				{
					int imageX = (x - 7 / 2 + filterX + iWidth) % iWidth;
					int imageY = (y - 7 / 2 + filterY + iHeight) % iHeight;
					LoG[imageY * iWidth + imageX] = LoG[imageY * iWidth + imageX] + (final1[filterY][filterX] * pImg[imageY * iWidth + imageX]) / 49;
					if (LoG[imageY * iWidth + imageX] < 0)
						LoG[imageY * iWidth + imageX] = 0;
					else if (LoG[imageY * iWidth + imageX] >255)
						LoG[imageY * iWidth + imageX] = 255;
				}
		}

	for (int i = 3; i<iHeight - 3; i++)
		for (int j = 3; j < iWidth - 3; j++)
		{
			pImg[i*Wp + j] = LoG[i*Wp + j];
		}

	OnDraw(GetDC());


}

void CKingimageView::OnLogmask()
{
	// 11x11 MASK

	CKingimageDoc* pDoc = GetDocument();

	int iBitPerPixel = pDoc->_bmp->bitsperpixel;
	int iWidth = pDoc->_bmp->width;
	int iHeight = pDoc->_bmp->height;
	BYTE *pImg = pDoc->_bmp->point;

	int Wp = iWidth;
	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		int r = iWidth % 4;
		int p = (4 - r) % 4;
		Wp = iWidth + p;
	}
	else if (iBitPerPixel == 24)	// RGB image
	{
		int r = (3 * iWidth) % 4;
		int p = (4 - r) % 4;
		Wp = 3 * iWidth + p;
	}

	std::ofstream ofs1;
	ofs1.open("Mask2.txt", std::ofstream::out | std::ofstream::app);

	int i, j, m, n;
	double *LoG = new double[Wp*iHeight];
	double sigma2 = 5.0 * 5.0;
	double sigma4 = 5.0 * 5.0 * 5.0 * 5.0;
	double con = 2 * 3.14*sigma4;
	int final1[11][11] = { 0 };

	for (int x = 0; x < iWidth; x++)
		for (int y = 0; y < iHeight; y++)
		{
			LoG[x*iWidth + y] = 0;
		}

	for (i = -5, m = 0; i <= 5, m<11; i++, m++)
	{
		for (j = -5, n = 0; j <= 5, n<11; j++, n++)
		{
			double a = (i*i + j*j - 2.0 * sigma2);
			double b = exp(-(i*i + j*j) / (2.0 * sigma2));
			double final = (((a*b) / con) * -400);
			final1[m][n] = trunc(final);
			ofs1 << final1[m][n] << "\t";

		}
		ofs1 << "\n";

	}


	for (int x = 0; x < iWidth; x++)
		for (int y = 0; y < iHeight; y++)
		{
			for (int mask_y = 0; mask_y < 11; mask_y++)
				for (int mask_x = 0; mask_x < 11; mask_x++)
				{
					int img_x = (x - 11 / 2 + mask_x + iWidth) % iWidth;
					int img_y = (y - 11 / 2 + mask_y + iHeight) % iHeight;
					LoG[img_y * iWidth + img_x] = LoG[img_y * iWidth + img_x] + (final1[mask_y][mask_x] * pImg[img_y * iWidth + img_x]) / 121;
					if (LoG[img_y * iWidth + img_x] < 0)
						LoG[img_y * iWidth + img_x] = 0;
					else if (LoG[img_y * iWidth + img_x] >255)
						LoG[img_y * iWidth + img_x] = 255;
				}
		}

	for (int i = 5; i<iHeight - 5; i++)
		for (int j = 5; j < iWidth - 5; j++)
		{
			pImg[i*Wp + j] = LoG[i*Wp + j];
		}

	OnDraw(GetDC());





}

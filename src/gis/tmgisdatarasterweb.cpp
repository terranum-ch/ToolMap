/***************************************************************************
 tmgisdatarasterweb
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tmgisdatarasterweb.h"
#include "../gui/tmwebframe.h"
#include "tmrenderer.h"

/*
const double tmGISDataRasterWeb::m_AvaillableResolutions [] ={
    156543.03390625, 78271.516953125, 39135.7584765625,
    19567.87923828125, 9783.939619140625, 4891.9698095703125,
    2445.9849047851562, 1222.9924523925781, 611.4962261962891,
    305.74811309814453, 152.87405654907226, 76.43702827453613,
    38.218514137268066, 19.109257068634033, 9.554628534317017,
    4.777314267158508, 2.388657133579254, 1.194328566789627,
    0.5971642833948135, 0.29858214169740677, 0.14929107084870338,
    0.07464553542435169
};
*/


tmGISDataRasterWeb::tmGISDataRasterWeb(){
	m_FileType = _T("Web Raster");
	m_ClassType = tmGIS_RASTER_WEB;
    m_WebFrameRef = NULL;
}

tmGISDataRasterWeb::~tmGISDataRasterWeb(){
}


wxString tmGISDataRasterWeb::GetDataSizeAsHtml (int iPrecision){
	wxString myResult = _("<U><B>Web Raster Size</B></U><BR>No Size!");
	return myResult;
}

/*
bool tmGISDataRasterWeb::Open (const wxString & filename, bool bReadWrite){
    tmGISData::Open(filename, bReadWrite);
	wxASSERT(m_DataSet == NULL);
    
    if (m_WebFrameRef == NULL) {
        return false;
    }
    m_WebFrameRef->LoadPage(filename);
    while ( m_WebFrameRef->IsLoaded() == false );
    return true;
}



tmRealRect tmGISDataRasterWeb::GetMinimalBoundingRectangle(){
    return tmRealRect(0,0,0,0);
}*/



bool tmGISDataRasterWeb::SetSpatialFilter (tmRealRect filter, int type){
	// check if view moved !
	if (m_FilterCoordLocal == filter){
		return true;
	}
	m_FilterCoordLocal = filter;
    
    // convert project coordinates into web coordinates and loads data into tmwebframe
    wxRealPoint xymin = GetCoordConvert()->GetPointGoogle(wxRealPoint(filter.x_min, filter.y_min));
    wxRealPoint xymax = GetCoordConvert()->GetPointGoogle(wxRealPoint(filter.x_max, filter.y_max));
    m_FilterCoordWeb = tmRealRect (xymin.x, xymin.y, xymax.x, xymax.y);
    
	wxLogMessage(_("web coordinates: %f, %f, %f, %f."), xymin.x, xymin.y, xymax.x, xymax.y);

    // get image information
    tmRealRect myImgRealCoord = GetMinimalBoundingRectangle();
    if (myImgRealCoord == tmRealRect(0,0,0,0)){
        return false;
    }

    if(myImgRealCoord.Clip(m_FilterCoordWeb, m_ClippedCoord)) {
        // image visible,
        // clip image with spatial filter
        m_PxImgFilter = ConvertClipedImage(myImgRealCoord, m_ClippedCoord);
    }
    wxLogDebug(_T("web pixels : %d, %d, %d, %d"), m_PxImgFilter.x, m_PxImgFilter.y, m_PxImgFilter.GetWidth(), m_PxImgFilter.GetHeight());
    return true;

    if (m_WebFrameRef == NULL) {
        return true;
    }
    
    m_WebFrameRef->ZoomToExtend(m_FilterCoordWeb);
	while (m_WebFrameRef->HasZoomed() == false){
        continue; // wait for the loading of the raster
    }
    
    // Resolution
    double myResolution = _GetResolution(m_FilterCoordWeb);
    double myClosestResolution = _GetClosestAvaillableResolution(m_FilterCoordWeb, myResolution);
    wxLogMessage(_("Actual Resolution= %f, Closest Resolution= %f"), myResolution, myClosestResolution);
    
    double myResolutionDiff = myClosestResolution-myResolution;
    wxSize myClientSize = m_WebFrameRef->GetClientSize();
    double xdiff = myResolutionDiff * myClientSize.GetWidth();
    double ydiff = myResolutionDiff * myClientSize.GetHeight();
    
    tmRealRect myWebCoord (m_FilterCoordWeb);
    myWebCoord.x_max += xdiff / 2.0;
    myWebCoord.y_max += ydiff / 2.0;
    myWebCoord.x_min -= xdiff / 2.0;
    myWebCoord.y_min -= ydiff / 2.0;
    
    //wxLogMessage(_("x difference= %f, y difference= %f"), xdiff , ydiff );
    //wxLogMessage(_("web coord: %f, %f, %f, %f,"), myWebCoord.x_min, myWebCoord.y_min, myWebCoord.x_max, myWebCoord.y_max);
    //wxLogMessage(_("x diffpx= %f, y diffpx= %f"), xdiff / myClosestResolution, ydiff / myClosestResolution);
    return true;
}


/*
double tmGISDataRasterWeb::_GetClosestAvaillableResolution(tmRealRect bounds, double resolution){
    int iNbResolutions = sizeof(m_AvaillableResolutions) / sizeof(double);
    for (int i = 0; i< iNbResolutions; i++) {
        if (m_AvaillableResolutions[i] < resolution) {
            return m_AvaillableResolutions[i-1];
        }
    }
    return wxNOT_FOUND;
}


double tmGISDataRasterWeb::_GetResolution (tmRealRect bounds){
    wxSize myClientSize = m_WebFrameRef->GetClientSize();
    return std::max((bounds.x_max - bounds.x_min) / myClientSize.GetWidth(),
                    (bounds.y_max - bounds.y_min) / myClientSize.GetHeight());
}



bool tmGISDataRasterWeb::IsImageInsideVisibleArea (){
    // return always true. Web image covers the world.
    return true;
}
*/

/*
CPLErr tmGISDataRasterWeb::GetImageData(unsigned char **imgbuf, unsigned int   *imglen,
                                        unsigned char **maskbuf, unsigned int   *masklen,
                                        wxSize imgSize){
    // my definitions
    int bEnablem_DataSettAlpha = FALSE; //, bEnableSrcAlpha = FALSE;
    int bMakeMask = false, bMakeAlpha = false, bInvertMask = false;
    wxRect imgfilter = m_PxImgFilter;

    double dmin = 0;
    double dmax = 0;
    double dnodata = 0;


    CPLErr ret = CE_None;

    GDALColorTable *pal = NULL;


    m_DataSet->FlushCache();

    int rasterCount  = m_DataSet->GetRasterCount();
    int nRasterXSize = imgfilter.GetWidth();		//m_DataSet->GetRasterXSize();
    int nRasterYSize = imgfilter.GetHeight();	//m_DataSet->GetRasterYSize();
    if ( ! (nRasterXSize > 0 && nRasterYSize > 0 ))
    {
        if (IsLoggingEnabled()){
            wxLogMessage(_("The dimensions (%ix%i) are invalid"),
                         nRasterXSize, nRasterYSize);
        }
        return CE_Failure;
    }

    //
    // create the new image array for RGBRGB... values
    //
    *imglen = 3 * imgSize.GetWidth() * imgSize.GetHeight();
    *imgbuf = (unsigned char*)CPLMalloc(*imglen);
    if ( *imgbuf == NULL)
    {
        if (IsLoggingEnabled()){
            wxLogMessage(_("The system does not have enough memory to project"));
        }
        return CE_Failure;
    }

    //
    // if there are three or more banm_DataSet assume that the first three
    // are for RGB, unless told otherwise
    //
    if (rasterCount >= 3)
    {
        for (int i=1; i <= 3; i++)
        {
            int offs = 0;
            GDALRasterBand *band = m_DataSet->GetRasterBand(i);

            switch (band->GetColorInterpretation())
            {
                case GCI_Undefined: offs = i-1; break;
                case GCI_RedBand:   offs = 0; break;
                case GCI_GreenBand: offs = 1; break;
                case GCI_BlueBand:  offs = 2; break;
                default:            offs = -1; break;
            }

            //
            // copy the image into the buffer using the proper offset
            // so we first copy over all Red values, then all Green
            // values, and then all Blue values
            //

            if (0 <= offs && offs < 3)
            {
                ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                     nRasterXSize, nRasterYSize,
                                     *imgbuf+offs, imgSize.GetWidth(), imgSize.GetHeight(),
                                     GDT_Byte, 3, 0);
                if (ret == CE_Failure)
                {
                    if (IsLoggingEnabled()){
                        wxLogError(_T( "An unknown error occured while reading band %i"),i);
                    }
                    break;
                }
            }
        }
    }
    else if (rasterCount >= 1)
    {
        //
        // one band is either a palette based image, or greyscale
        //

        // definitions for grayscale
        GDALRasterBand *band = m_DataSet->GetRasterBand(1);
        int iLoop = 0;
        double dRange = 0;
        int iBuffSize = 0;
        void * myGdalScanData = NULL;
        unsigned char *data = NULL;
        //char Resultat = '\n';
        GDALDataType myDataType;

        switch (band->GetColorInterpretation())
        {
            case GCI_PaletteIndex:

                pal = band->GetColorTable();

                if (pal == NULL)
                {
                    if (IsLoggingEnabled()){
                        wxLogError(_T( "Couldn't find a palette for palette-based image") );
                    }
                    ret = CE_Failure;
                }
                else
                {
                    GDALPaletteInterp pal_interp
                            = pal->GetPaletteInterpretation();

                    //
                    // copy over all the palette indices and then
                    // loop through the buffer replacing the values
                    // with the correct RGB triples.
                    //
                    ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                         nRasterXSize, nRasterYSize,
                                         *imgbuf, imgSize.GetWidth(), imgSize.GetHeight(),
                                         GDT_UInt16, 3, 0);

                    if (ret == CE_Failure)
                    {
                        if (IsLoggingEnabled()){
                            wxLogError(_T("An unknown error occured while reading band 1"));
                        }
                        break;
                    }

                    for (unsigned char *data = *imgbuf;
                         data != (*imgbuf+*imglen);
                         data += 3)
                    {

                        unsigned short int val = *((unsigned short int *)data);

                        const GDALColorEntry *color = pal->GetColorEntry(val);

                        if (pal_interp == GPI_Gray)
                        {
                            *(data + 0) = color->c1;
                            *(data + 1) = color->c1;
                            *(data + 2) = color->c1;
                        }
                        else
                        {
                            *(data + 0) = color->c1;
                            *(data + 1) = color->c2;
                            *(data + 2) = color->c3;
                        }
                    }
                }
                break;

            case GCI_Undefined: // can we try to make a greyscale image?
            case GCI_GrayIndex:

                // compute stats,
                // TODO: move stat computation in another place
                GetStatMinMaxNoDataValue(dmin, dmax, dnodata);
                dRange = dmax - dmin;

                myDataType = band->GetRasterDataType();
                myGdalScanData = ReadImageData(band, imgfilter, imgSize, iBuffSize);
                data = *imgbuf;
                for (unsigned int i = 0; i<*imglen; i += 3)
                {

                    double myGrayValDouble = ReadGDALValueToDouble(myGdalScanData,
                                                                   myDataType, iLoop);
                    iLoop ++;

                    int myGrayValInt = static_cast < int >((myGrayValDouble - dmin) * (255 / dRange));
                    if (myGrayValInt < 0) myGrayValInt = 0;
                    if (myGrayValInt > 255) myGrayValInt = 255;

                    //TODO: Dealing with no-data here

                    *(data + i) = myGrayValInt;
                    *(data + i + 1) = myGrayValInt;
                    *(data + i + 2) = myGrayValInt;

                }

                CPLFree (myGdalScanData);
                break;

            default:
                if (IsLoggingEnabled()){
                    wxLogError(_T("Unsupported color interpretation '%s'"),
                               GDALGetColorInterpretationName(band->GetColorInterpretation()));
                }
                ret = CE_Failure;
                break;
        }
    }
    else
    {
        if (IsLoggingEnabled()){
            wxLogError(_T("Unsupported number of raster banm_DataSet (%i) in image"),
                       rasterCount);
        }
        ret = CE_Failure;
    }

    if (ret == CE_None && bEnablem_DataSettAlpha && rasterCount > 1)
    {
        if (bMakeMask)
        {
            //
            // The mask is really an XBM image. In other worm_DataSet, each
            // pixel is represented by one bit in a byte array.
            //
            // First read the alpha band, and then convert it to
            // a bit array by thresholding each pixel value at 128.
            //

            *masklen = ((imgSize.GetWidth() + 7) / 8) * imgSize.GetHeight();
            *maskbuf = (unsigned char *)CPLMalloc(*masklen);

            if ( *maskbuf != NULL )
            {
                unsigned char *tmp
                        = (unsigned char *)CPLMalloc(imgSize.GetWidth() * imgSize.GetHeight());

                if ( tmp == NULL )
                {
                    CPLFree(*maskbuf);
                    *maskbuf = NULL;
                }
                else
                {
                    GDALRasterBand *band = m_DataSet->GetRasterBand(rasterCount);

                    ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                         nRasterXSize, nRasterYSize,
                                         tmp, imgSize.GetWidth(), imgSize.GetHeight(),
                                         GDT_Byte, 0, 0);

                    if (ret != CE_Failure)
                    {
                        int i, j, b=1, c=0;
                        unsigned char *ptr = *maskbuf;
                        unsigned char *tptr = tmp;

                        //unsigned int empty_count=0;

                        for (i=0; i < nRasterYSize; i++)
                        {
                            for (j=nRasterXSize; j >= 8; j -= 8)
                            {
                                c=0; b=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;

                                if (bInvertMask)
                                    *(ptr++) = ~c;
                                else
                                    *(ptr++) = c;
                            }

                            c=0; b=1;
                            switch (nRasterXSize & 7)
                            {
                                case 7: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 6: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 5: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 4: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 3: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 2: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 1:
                                    if (*tptr++ >= 128) {c|=b;}
                                    b<<=1;

                                    //
                                    // byte should be padded with 0's so
                                    // it's not a simple inversion
                                    //
                                    if (bInvertMask)
                                        *(ptr++) = ~c & (b-1);
                                    else
                                        *(ptr++) = c;

                                default: break;
                            }
                        }

#if 0
                        if (empty_count == *masklen)
                        {
                            fprintf(stderr, "mask not used\n");

                            CPLFree(*maskbuf);
                            *maskbuf = NULL;
                        }
#endif
                    }

                    CPLFree(tmp);
                    tmp = NULL;
                }

            }
        }
        else if (bMakeAlpha)
        {
            //
            // This is the simple case. The array we get back from RasterIO
            // is already in the correct format.
            //

            *masklen = imgSize.GetWidth() * imgSize.GetHeight();
            *maskbuf = (unsigned char *)CPLMalloc(*masklen);

            if ( *maskbuf != NULL )
            {
                GDALRasterBand *band = m_DataSet->GetRasterBand(rasterCount);

                ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                     nRasterXSize, nRasterYSize,
                                     *maskbuf, imgSize.GetWidth(), imgSize.GetHeight(),
                                     GDT_Byte, 0, 0);

#if 0
                if (ret == CE_Failure)
                {
                    CPLFree(*maskbuf);
                    *maskbuf = NULL;
                }
#endif
            }
        }
    }

    if (ret != CE_None)
    {
        if (*imgbuf  != NULL) { CPLFree(*imgbuf);  *imgbuf  = NULL; }
        if (*maskbuf != NULL) { CPLFree(*maskbuf); *maskbuf = NULL; }
    }

    return ret;
}
*/

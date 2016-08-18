// This is not The Greatest Code in the World, no! This is just a tribute!
//
// Re-Typeset program for converting scanned documents to smaller size of page
// without changing text size.
//
// Copyright (C) 2013-2015 Piotr Mika (piotr.mt.mika@gmail.com).
// Copyright (C)      2015 Marcin Garbiak (marcin.garbiak@gmail.com).
//
// This  program is free software:  you can  redistribute it and/or  modify it
// under the terms of the  GNU General Public License as published by the Free
// Software Foundation,  either version 3 of the License,  or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY  WARRANTY;  without  even the implied  warranty  of MERCHANTABILITY  or
// FITNESS FOR  A PARTICULAR  PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the  GNU  General  Public  License along
// with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef VERSION_HPP
#define VERSION_HPP
//# This file is used  as a source to create release notes with following cmd:
//# sh>  cat version.hpp | grep -v '#' | sed -e 's!//!!g' > RELEASENOTES.TXT .
//# All lines with  ``#''  character  will be removed  from  destination file. 
//# Comment  marks  ``//''  will be  deleted also.  Main  goal  is to  provide 
//# versions history with typical free software header for end user.
#ifndef COMPILATION
#define COMPILATION "debug"
#endif

// Dependencies for dynamically linked builds:
//    qt5
//    qt5-base
//    qt5-imageformats (for tiff support)

#define VERSION "1.1.0"
//# MAJOR.MINOR.PATCH understand as in http://semver.org/spec/v2.0.0.html

// Versions history:
// 1.1.0: New features available:  live configuration of algorithm parameters,
//        histogram equalisation for images (beta),  new table of contents and
//        title page -- user can write author and title of book.  Additionally
//        these values can be read form directory's name with specific syntax:
//        `` Author_Name--Book_Title ''.  Separator  is  double  hyphen  (--).
//        Underscore is replaced to space. Space in name is allowed too.
//
// 1.0.0: All basic functionality is working. Page segmentation have following
//        features:  finding  lines and  words with  searching of  images with
//        descriptions,  divided words  and new  paragraphs;  finding headers,
//        footers  and  page numbering.  Re-typesetting  can use  all of these
//        informations  to create  new pages with additional features:  adding
//        book reading progress bar and text justification.
//        


#endif // VERSION_HPP

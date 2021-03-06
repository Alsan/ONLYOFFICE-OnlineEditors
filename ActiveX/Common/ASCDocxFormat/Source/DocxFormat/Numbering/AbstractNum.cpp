﻿/*
 * (c) Copyright Ascensio System SIA 2010-2014
 *
 * This program is a free software product. You can redistribute it and/or 
 * modify it under the terms of the GNU Affero General Public License (AGPL) 
 * version 3 as published by the Free Software Foundation. In accordance with 
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect 
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For 
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under 
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
 
#include "precompiled_docxformat.h"


#include "./../Numbering.h"
#include "Exception/log_runtime_error.h"

namespace OOX
{
	Numbering::AbstractNum::AbstractNum()
	{
		Id	=	0;
	}

	Numbering::AbstractNum::~AbstractNum()
	{

	}

	Numbering::AbstractNum::AbstractNum(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::AbstractNum& Numbering::AbstractNum::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::AbstractNum::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( _T("w:abstractNum") == oNode.GetName() )
		{
			Id = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:abstractNumId"))));

			XmlUtils::CXmlNode oChild;
			if ( oNode.GetNode( _T("w:nsid"), oChild ) )
				Nsid = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:multiLevelType"), oChild ) )
				MultiLevelType = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:tmpl"), oChild ) )
				Tmpl = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:numStyleLink"), oChild ) )
				numStyleLink = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

			XmlUtils::CXmlNodes oLvlList;
			if ( oNode.GetNodes( _T("w:lvl"), oLvlList ) )
			{
				XmlUtils::CXmlNode oLvlNode;
				for ( int nIndex = 0; nIndex < oLvlList.GetCount(); ++nIndex )
				{
					if ( oLvlList.GetAt( nIndex, oLvlNode ) )
					{
						OOX::Numbering::Level oLvl;
						oLvl.fromXML(oLvlNode);
						Levels.push_back(oLvl);
					}
				}
			}
		}
	}

	const Numbering::Level Numbering::AbstractNum::getLevel(const int numLevel) const
	{
		for (std::vector<Level>::const_iterator iter = Levels.begin(); iter != Levels.end(); ++iter)
		{
			if ((*iter).Ilvl == numLevel)
				return (*iter);
		}

		throw log_runtime_error("bad abstractNum");
	}

} // namespace OOX
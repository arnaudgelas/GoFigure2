void
Lsm3DSerieImport::
SetFileName( char * name )
{
    if ( this->m_FileName && name && (!strcmp(this->m_FileName,name)))
      {
      return;
      }
    if (!name && !this->m_FileName)
      {
      return;
      }
    if (this->m_FileName)
      {
      delete [] this->m_FileName;
      }
    if (name)
      {
      this->m_FileName = new char[strlen(name) + 1];
      strcpy(this->m_FileName, name);
      }
    else
      {
      this->m_FileName = NULL;
      }
    this->Modified();
}
		
FileListType* 
Lsm3DSerieImport::
GetOutput()
{
  return( &(this->m_OutputFileList) );
};
		
void 
Lsm3DSerieImport::
Update(void)
{
  Glob();
  CreateOutput();
};
 
void 
Lsm3DSerieImport::
SetGroupId( int Uservalue )
{ 
  this->m_GroupId = Uservalue; 
  this->Modified(); 
};
		
void 
Lsm3DSerieImport::
Glob()
{
    m_numGroupStart.clear();
    m_numGroupLength.clear();
		
    // glob all jpeg file names
    std::string unixArchetype = m_FileName;
    itksys::SystemTools::ConvertToUnixSlashes(unixArchetype);
    if (itksys::SystemTools::FileIsDirectory( unixArchetype.c_str() ))
      {
      return;
      }
			
    // Parse the fileNameName and fileNamePath
   std::string origFileName = 
   itksys::SystemTools::GetFilenameName( unixArchetype.c_str() );
   std::string fileNamePath = 
   itksys::SystemTools::GetFilenamePath( unixArchetype.c_str() );
   std::string pathPrefix;
			
   // "Clean" the filename by escaping any special characters with backslashes.
   // This allows us to pass in filenames that include these special characters.
   std::string fileName;
   for( unsigned int j = 0; j < origFileName.length(); j++ )
     {
     char oneChar = origFileName[j];
     if( 
       oneChar == '^' ||
       oneChar == '$' ||
       oneChar == '.' ||
       oneChar == '[' ||
       oneChar == ']' ||
       oneChar == '-' ||
       oneChar == '*' ||
       oneChar == '+' ||
       oneChar == '?' ||
       oneChar == '(' ||
       oneChar == ')' )
      {
      fileName += "\\";
      }
    fileName += oneChar;
    }
			
  // If there is no "/" in the name, the directory is not specified.
  // In that case, use the default ".". 
  // This is necessary for the RegularExpressionSeriesFileNames.
  if (fileNamePath == "")
    {
    fileNamePath = ".";
    pathPrefix = "./";
    }
  else
    {
    pathPrefix = "";
    }
			
  std::string regExpString = "([0-9]+)";
  int sIndex;
  // parse and keep it for ouput generation
  std::string::iterator sit;
  for (sit = fileName.begin(); sit < fileName.end(); sit++)
    {
    // If the element is a number, find its starting index and length.
    if ((*sit) >= '0' && (*sit) <= '9')
      {
      sIndex = static_cast< int >( sit - fileName.begin() );
      m_numGroupStart.push_back( sIndex );
					
      // Loop to one past the end of the group of numbers.
      while ( sit != fileName.end() && (*sit) >= '0' && (*sit) <= '9' )
        {
        ++sit;
        }
					
      m_numGroupLength.push_back( static_cast< int >(sit - fileName.begin()) - sIndex );
					
      if( sit == fileName.end() )
        {
        break;
        }
      }
    }
			
  // create the regular expression to glob the entire set of file
  std::string regExpFileName = fileName;
  IntVectorType::reverse_iterator numGroupLengthItr = m_numGroupLength.rbegin();
  IntVectorType::reverse_iterator numGroupStartItr  = m_numGroupStart.rbegin();
  int NumGroupCounter = 0;
  while( numGroupLengthItr != m_numGroupLength.rend() &&
    numGroupStartItr != m_numGroupStart.rend() )
    {
    if( NumGroupCounter  == m_GroupId  )
      {
      regExpFileName.replace(*numGroupStartItr,*numGroupLengthItr,regExpString);
      break;
      }
    ++numGroupLengthItr;
    ++numGroupStartItr;
    ++NumGroupCounter;
    }
			
  // Include only filenames that exactly match this regular expression.  Don't
  // match filenames that have this string as a substring (ie. that have extra
  // prefixes or suffixes).
  regExpFileName = "^" + regExpFileName + "$";
			
  // Use a RegularExpressionSeriesFileNames to find the files to return
  itk::RegularExpressionSeriesFileNames::Pointer fit = itk::RegularExpressionSeriesFileNames::New();
  fit->SetDirectory( fileNamePath.c_str() );
  fit->SetRegularExpression( regExpFileName.c_str() );
  fit->SetSubMatch(1);
  fit->NumericSortOn();
  m_FileNameS = fit->GetFileNames();
};
		
		
void 
Lsm3DSerieImport::
CreateOutput()
{
  std::vector<std::string>::iterator nit;
  for( nit = m_FileNameS.begin();
    nit != m_FileNameS.end();
    nit++)
    {
    GoFigureFileInfoHelper tempInfo;
    tempInfo.Filename = (*nit);
    std::string origFileName = 
    itksys::SystemTools::GetFilenameName( (*nit).c_str() );
		
    IntVectorType::reverse_iterator numGroupLengthItr = 
      m_numGroupLength.rbegin();
    IntVectorType::reverse_iterator numGroupStartItr  = 
      m_numGroupStart.rbegin();
    int NumGroupCounter = 0;
    while( numGroupLengthItr != m_numGroupLength.rend() &&
      numGroupStartItr != m_numGroupStart.rend() )
      {
      if( NumGroupCounter  == m_GroupId  )
        {
        std::string ValueAsString( 
          origFileName, 
          (*numGroupStartItr)-(1-NumGroupCounter), 
          (*numGroupLengthItr) ); 
        tempInfo.TimePoint = atof( ValueAsString.c_str() );
        m_OutputFileList.push_back( tempInfo );
        break;
        }
      ++numGroupLengthItr;
      ++numGroupStartItr;
      ++NumGroupCounter;
				
      } // end for each numerical group
				
    } // end for each filename
			
    m_FileNameS.clear();
    std::sort( m_OutputFileList.begin(), m_OutputFileList.end() );
			
    FileListType::iterator myIt = m_OutputFileList.begin();
    while( myIt != m_OutputFileList.end() )
      {
      itkDebugMacro( 
		<< (*myIt).Filename\
        << " " << (*myIt).Channel\
        << " " << (*myIt).TimePoint\
        << " " << (*myIt).ZDepth );     
      myIt++;
      }
};
		
Lsm3DSerieImport::
~Lsm3DSerieImport() 
{
  if (this->m_FileName)
    {
    delete [] this->m_FileName;
    this->m_FileName = NULL;
    }
};
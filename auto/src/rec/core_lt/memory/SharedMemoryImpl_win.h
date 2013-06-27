//  Copyright (C) 2004-2009, Robotics Equipment Corporation GmbH

#include <windows.h>

class SharedMemoryImpl
{
public:
  HANDLE hFile;
  LPINT hView;
  HANDLE memlock;
};

template< typename sharedType >
SharedMemory< sharedType >::SharedMemory( int key)
{
  impl = new SharedMemoryImpl();

  this->key = key;
  impl->hFile = NULL;
  impl->hView = NULL;
  impl->memlock = NULL;
  data = NULL;

  wchar_t filename[32];
  swprintf( filename, 32, L"SharedMemory%i", key );

  //Try to open an existing memory segment
  impl->hFile = OpenFileMapping( FILE_MAP_ALL_ACCESS, 
    FALSE,
		filename);            

  if (impl->hFile != NULL)
  {
    //Open semaphore
    wchar_t semname[16];
    swprintf( semname, 16, L"REC%i", key );
    impl->memlock = OpenSemaphore( SEMAPHORE_ALL_ACCESS, FALSE, semname );
    if (impl->memlock == NULL)
      throw rec::core_lt::Exception( "Unable to open semaphore." );

    owner = false;
  }
  else
  {
    //No exiting memory segment found. Create a new one
    impl->hFile = CreateFileMapping(INVALID_HANDLE_VALUE, 
      NULL,
		  PAGE_READWRITE, 
		  0, 
		  sizeof(sharedType), 
      filename);

	  if (impl->hFile == NULL)
      throw rec::core_lt::Exception( "Unable to create a shared memory segment." );

    //Create semaphore
    wchar_t semname[16];
    swprintf( semname, 16, L"REC%i", key);
    impl->memlock = CreateSemaphore( NULL, 1, 1, semname);
	  if (impl->memlock == NULL)
      throw rec::core_lt::Exception("Unable to create semaphore.");

    owner = true;

  }

  impl->hView = (LPINT) MapViewOfFile(impl->hFile, 
	 FILE_MAP_ALL_ACCESS,  
	 0,
	 0,
	 0);

  if (impl->hView == NULL)
    throw rec::core_lt::Exception("Unable to create a View.");

  data = (sharedType*)impl->hView;

  if( owner )
  {    
    // in-place new to initialize share-memory
    new( data ) sharedType;
  }
  //if( owner )
  //  std::cout << "Shared memory has been successfully created!" << std::endl;
  //else
  //  std::cout << "Shared memory has been successfully opened!" << std::endl;
}

template< typename sharedType >
SharedMemory< sharedType >::~SharedMemory()
{
  //Close shared memory segment
  if( impl->hFile == NULL )
    return;

	if( !UnmapViewOfFile( impl->hView ) ) 
    throw rec::core_lt::Exception( "Could not unmap view of file." );
	
	CloseHandle( impl->hFile ); 

  //Close semaphore
  CloseHandle( impl->memlock );

  impl->hView = NULL;
  impl->hFile = NULL;
  data = NULL;
  impl->memlock = NULL;
    
  delete impl;

//   std::cout << "Shared memory has been successfully destroyed!" << std::endl;
}

template< typename sharedType >
void SharedMemory< sharedType >::lock()
{
  if( WAIT_FAILED == WaitForSingleObject( impl->memlock, INFINITE ) )
  {
    throw rec::core_lt::Exception( "lock failed" );
  }
}

template< typename sharedType >
void SharedMemory< sharedType >::unlock()
{
  if( ReleaseSemaphore ( impl->memlock, 1, NULL) != TRUE )
  {
    throw rec::core_lt::Exception( "unlock failed" );
  }
}

template< typename sharedType >
sharedType* SharedMemory< sharedType >::getData() const
{
  return data;
}

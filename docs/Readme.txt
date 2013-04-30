Building for the first time:
- To complete the first build you need to copy the entire DirectX SDK (July 2010) to the 3DApp/DX folder.
  This is how you showld be seeing you DX folder.
	3DApp
        --DX
        ----DevRun (All the .dll files in Developer Runtime)
        ------x64
	------x86
	----DXUT
	------Core
	------Optional
	----DXUT11
	------Core
	------Optional
	----Include
	----Lib
	------x64
	------x86
- Also you need to copy D3DX10d_43.dll (inside DevRun) to the corresponding 3DApp/Build/bin folder created after building.
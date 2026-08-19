
# DCOM Upload & Execute

  

DCOM Lateral movement POC abusing the IMsiServer interface

https://www.deepinstinct.com/blog/forget-psexec-dcom-upload-execute-backdoor

The solution contains 2 projects

  

-  **DCOMUploadExec** - A C++ project which hosts the lateral movement attack code

-  **AssemblyPayload** - A .NET DLL that pops a MessageBox. It is the default payload **DCOMUploadExec** uses

## Build

`DCOMUploadExec` and `AssemblyPayload` should be built separately:

1. Build `DCOMUploadExec` with Visual Studio using toolset `v145`, for example `Release|x64`.
2. Build `AssemblyPayload` separately with MSBuild and MSVC x64/x86 build tools `14.29`, for example:
   - `MSBuild.exe AssemblyPayload\AssemblyPayload.csproj /p:Configuration=Release /p:Platform=x64`

The default payload output is `AssemblyPayload\bin\Release\payload.dll`.

  

## Basic Usage

  

Compile the solution

`Usage: DCOMUploadExec.exe [domain]\[user]:[password]@[address]`

`Local Usage: DCOMUploadExec.exe LOCALHOST (Run this as administrator)`

  

DCOMUploadExec.exe will:

- Upload the default payload DLL, **AssemblyPayload**, to the remote PC

- Load the DLL to a remote MSIEXEC.exe process

- Execute an export from the DLL

- Receive a result from the export

  

## Payload Configuration

  

The payload must be a [strong-named](https://learn.microsoft.com/en-us/dotnet/standard/assembly/strong-named) .NET assembly

  

**PayloadConfig.h** is used to configure the payload **DCOMUploadExec** will use.

It is defaulted to the output of **AssemblyPayload**.

  

In order to use a custom payload:

1. create a [strong-named](https://learn.microsoft.com/en-us/dotnet/standard/assembly/strong-named) .NET assembly that exports a function named *InitializeEmbeddedUI* - this will be the function that **DCOMUploadExec** will eventually execute on the remote target

  

2. Fill your custom payload's details in **PayloadConfig.h**:

  

   - PAYLOAD_ASSEMBLY_PATH = // local path to your created payload
   
     
   
   - ASSEMBLY_NAME = // the assembly name
   
     
   
   - ASSEMBLY_BITNESS = // "64" for an x64 payload, "32" for x86 payload;
   
     
   
   - ASSEMBLY_VERSION = // the value from `sigcheck.exe -n
   PAYLOAD_ASSEMBLY_PATH`
   
   -  ASSEMBLY_PUBLIC_KEY = // [the value
   from](https://learn.microsoft.com/en-us/dotnet/framework/tools/sn-exe-strong-name-tool)
   `sn.exe -T PAYLOAD_ASSEMBLY_PATH`
 3. Compile **DCOMUploadExec** and use it

  

## Limitations

  

1. The attacker and victim machines must be in the same domain or forest.

  

2. The attacker and victim machines must be consistent with the [DCOM Hardening patch](https://support.microsoft.com/en-us/topic/kb5004442-manage-changes-for-windows-dcom-server-security-feature-bypass-cve-2021-26414-f1400b52-c141-43d2-941e-37ed901c769c) - either with the patch applied on both systems or absent on both.

3. The assembly payload must have a [strong-name](https://learn.microsoft.com/en-us/dotnet/standard/assembly/strong-named).

4. The assembly payload must be either x86 or x64 (Can't be AnyCPU).

  
  

## Credits

  
  

* [Eliran Nissan](https://x.com/eliran_nissan)
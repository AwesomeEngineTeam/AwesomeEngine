
set "dirPath=%cd%"
set "dirPath=%dirPath:\=\\%"

if not exist ".vs/" mkdir .vs

(
  echo {
  echo   "version": "0.2.1",
  echo   "defaults": {},
  echo   "configurations": [
  echo     {
  echo       "type": "default",
  echo       "project": "CMakeLists.txt",
  echo       "projectTarget": "RendererTest.exe (%dirPath%\\AwesomeEngine\\build\\Debug\\RendererTest.exe)",
  echo       "name": "RendererTest - Debug",
  echo       "currentDir": "${projectDir}\\AwesomeEngine"
  echo     },
  echo     {
  echo       "type": "default",
  echo       "project": "CMakeLists.txt",
  echo       "projectTarget": "RendererTest.exe (%dirPath%\\AwesomeEngine\\build\\Release\\RendererTest.exe)",
  echo       "name": "RendererTest - Release",
  echo       "currentDir": "${projectDir}\\AwesomeEngine"
  echo     },
  echo     {
  echo       "type": "default",
  echo       "project": "CMakeLists.txt",
  echo       "projectTarget": "RendererTest.exe (%dirPath%\\AwesomeEngine\\build\\RelWithDebInfo\\RendererTest.exe)",
  echo       "name": "RendererTest - RelWithDebInfo",
  echo       "currentDir": "${projectDir}\\AwesomeEngine"
  echo     },
  echo     {
  echo       "type": "default",
  echo       "project": "CMakeLists.txt",
  echo       "projectTarget": "Editor.exe (%dirPath%\\AwesomeEditor\\build\\Debug\\Editor.exe)",
  echo       "name": "Editor - Debug",
  echo       "currentDir": "${projectDir}\\AwesomeEditor"
  echo     },
  echo     {
  echo       "type": "default",
  echo       "project": "CMakeLists.txt",
  echo       "projectTarget": "Editor.exe (%dirPath%\\AwesomeEditor\\build\\Release\\Editor.exe)",
  echo       "name": "Editor - Release",
  echo       "currentDir": "${projectDir}\\AwesomeEditor"
  echo     },
  echo     {
  echo       "type": "default",
  echo       "project": "CMakeLists.txt",
  echo       "projectTarget": "Editor.exe (%dirPath%\\AwesomeEditor\\build\\RelWithDebInfo\\Editor.exe)",
  echo       "name": "Editor - RelWithDebInfo",
  echo       "currentDir": "${projectDir}\\AwesomeEditor"
  echo     }
  echo   ]
  echo }
) >".vs/launch.vs.json"

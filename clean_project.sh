#!/bin/bash

clean_project() {

  find . -iwholename '*cmake*' -not -name CMakeLists.txt -delete
  find . -iwholename '*Make*'  -not -name CMakeLists.txt -delete
  # Removes every executable except this file
  find . -type f -perm +111 -not -name "clean_project.sh" -delete

}

clean_project

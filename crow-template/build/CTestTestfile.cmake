# CMake generated Testfile for 
# Source directory: /home/thiago/Área de Trabalho/crow-template
# Build directory: /home/thiago/Área de Trabalho/crow-template/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(crow_test "/home/thiago/Área de Trabalho/crow-template/build/tests/unittest")
add_test(template_test "/home/thiago/Área de Trabalho/crow-template/build/tests/template/test.py")
set_tests_properties(template_test PROPERTIES  WORKING_DIRECTORY "/home/thiago/Área de Trabalho/crow-template/build/tests/template")
subdirs("src")
subdirs("tests")

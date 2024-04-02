add_test( PacketHandlerTest.TestAuthTcp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestAuthTcp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestAuthTcp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestReplyTcp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestReplyTcp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestReplyTcp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestJoinTcp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestJoinTcp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestJoinTcp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestMsgTcp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestMsgTcp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestMsgTcp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestErrTcp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestErrTcp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestErrTcp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestByeTcp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestByeTcp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestByeTcp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestAuthUdp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestAuthUdp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestAuthUdp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestConfirmUdp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestConfirmUdp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestConfirmUdp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestReplyUdp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestReplyUdp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestReplyUdp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestMsgUdp /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestMsgUdp]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestMsgUdp PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PacketHandlerTest.TestUdpReader /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=PacketHandlerTest.TestUdpReader]==] --gtest_also_run_disabled_tests)
set_tests_properties( PacketHandlerTest.TestUdpReader PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( UtilTest.TestParseMessage /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=UtilTest.TestParseMessage]==] --gtest_also_run_disabled_tests)
set_tests_properties( UtilTest.TestParseMessage PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( UtilTest.TestExtractTextFromByteArray /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=UtilTest.TestExtractTextFromByteArray]==] --gtest_also_run_disabled_tests)
set_tests_properties( UtilTest.TestExtractTextFromByteArray PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( UtilTest.TestGetIpAddrFromHost /mnt/e/intellijproject/test/IPK_project1/build/tests/chat_test [==[--gtest_filter=UtilTest.TestGetIpAddrFromHost]==] --gtest_also_run_disabled_tests)
set_tests_properties( UtilTest.TestGetIpAddrFromHost PROPERTIES WORKING_DIRECTORY /mnt/e/intellijproject/test/IPK_project1/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( chat_test_TESTS PacketHandlerTest.TestAuthTcp PacketHandlerTest.TestReplyTcp PacketHandlerTest.TestJoinTcp PacketHandlerTest.TestMsgTcp PacketHandlerTest.TestErrTcp PacketHandlerTest.TestByeTcp PacketHandlerTest.TestAuthUdp PacketHandlerTest.TestConfirmUdp PacketHandlerTest.TestReplyUdp PacketHandlerTest.TestMsgUdp PacketHandlerTest.TestUdpReader UtilTest.TestParseMessage UtilTest.TestExtractTextFromByteArray UtilTest.TestGetIpAddrFromHost)

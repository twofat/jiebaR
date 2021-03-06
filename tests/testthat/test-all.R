context("All tests")

test_that("segmentation", {
  cutter = worker("hmm")
  expect_identical({cutter <= "This is Test"}, c("This","is","Test"))
  cutter = worker("mix")
  expect_identical({cutter <= "这是一个测试"}, c("这是", "一个", "测试"))
  expect_identical({cutter <= "。、，；-！@#￥%……&*（）’"}, character(0))
  cutter = worker("tag")
  expect_identical({cutter <= "这是一个测试"},
                   structure(c("这是", "一个", "测试"), .Names = c("x", "m", "vn"
  )))
  cutter = worker("simhash")
  expect_identical({cutter <= "这是一个测试"},
                   structure(list(simhash = "1460457335435046831", 
                                  keyword = structure(c("测试", "这是", "一个"),
                                  .Names = c("7.14724", "4.29163", "2.81755"))),
                                  .Names = c("simhash", "keyword")))
  expect_identical({distance( "这是一个测试","这是一个测试",cutter)},
                   structure(list(distance = "0", 
                                  lhs = structure(c("测试", "这是","一个"), .Names = c("7.14724", "4.29163", "2.81755")), 
                                  rhs = structure(c("测试", "这是", "一个"), .Names = c("7.14724", "4.29163", "2.81755"))),
                             .Names = c("distance", "lhs", "rhs")))
  cutter = worker("keywords")
  expect_identical({cutter <= "这是一个测试"},
                   structure(c("测试", "这是", "一个"), .Names = c("7.14724", "4.29163", 
                                                             "2.81755")))
})

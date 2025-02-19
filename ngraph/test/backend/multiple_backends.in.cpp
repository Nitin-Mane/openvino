// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "gtest/gtest.h"
#include "ngraph/ngraph.hpp"
#include "ngraph/runtime/tensor.hpp"
#include "runtime/backend.hpp"
#include "util/all_close.hpp"
#include "util/all_close_f.hpp"
#include "util/known_element_types.hpp"
#include "util/ndarray.hpp"
#include "util/test_control.hpp"
#include "util/test_tools.hpp"

using namespace std;
using namespace ngraph;

static string s_manifest = "${MANIFEST}";

NGRAPH_TEST(${BACKEND_NAME}, multiple_backends) {
    Shape shape{2, 2};
    auto A1 = make_shared<op::Parameter>(element::f32, shape);
    auto B1 = make_shared<op::Parameter>(element::f32, shape);
    auto add = std::make_shared<op::v1::Add>(A1, B1);
    auto f = make_shared<Function>(add, ParameterVector{A1, B1});

    auto A2 = make_shared<op::Parameter>(element::f32, shape);
    auto B2 = make_shared<op::Parameter>(element::f32, shape);
    auto multiply = std::make_shared<op::v1::Multiply>(A2, B2);
    auto g = make_shared<Function>(multiply, ParameterVector{A2, B2});

    auto backend1 = runtime::Backend::create("${BACKEND_NAME}");

    auto backend2 = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    shared_ptr<runtime::Tensor> a1 = backend1->create_tensor(element::f32, shape);
    shared_ptr<runtime::Tensor> b1 = backend1->create_tensor(element::f32, shape);
    shared_ptr<runtime::Tensor> result1 = backend1->create_tensor(element::f32, shape);

    shared_ptr<runtime::Tensor> a2 = backend2->create_tensor(element::f32, shape);
    shared_ptr<runtime::Tensor> b2 = backend2->create_tensor(element::f32, shape);
    shared_ptr<runtime::Tensor> result2 = backend2->create_tensor(element::f32, shape);

    copy_data(a1, test::NDArray<float, 2>({{1, 2}, {3, 4}}).get_vector());
    copy_data(b1, test::NDArray<float, 2>({{5, 6}, {7, 8}}).get_vector());

    copy_data(a2, test::NDArray<float, 2>({{1, 2}, {3, 4}}).get_vector());
    copy_data(b2, test::NDArray<float, 2>({{5, 6}, {7, 8}}).get_vector());

    auto handle1 = backend1->compile(f);
    handle1->call_with_validate({result1}, {a1, b1});
    EXPECT_TRUE(
        test::all_close_f(read_vector<float>(result1), (test::NDArray<float, 2>({{6, 8}, {10, 12}})).get_vector()));

    auto handle2 = backend2->compile(g);
    handle2->call_with_validate({result2}, {a2, b2});
    EXPECT_TRUE(
        test::all_close_f(read_vector<float>(result2), (test::NDArray<float, 2>({{5, 12}, {21, 32}})).get_vector()));
}

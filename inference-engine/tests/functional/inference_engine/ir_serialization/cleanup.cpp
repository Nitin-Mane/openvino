// Copyright (C) 2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <fstream>

#include "common_test_utils/ngraph_test_utils.hpp"
#include "gtest/gtest.h"
#include "ie_core.hpp"
#include "ngraph_functions/builders.hpp"

class SerializationCleanupTest : public CommonTestUtils::TestsCommon {
protected:
    const std::string test_name = GetTestName() + "_" + GetTimestamp();
    std::string m_out_xml_path = test_name + ".xml";
    std::string m_out_bin_path = test_name + ".bin";

    void TearDown() override {
        std::remove(m_out_xml_path.c_str());
        std::remove(m_out_bin_path.c_str());
    }
};

namespace {
std::shared_ptr<ngraph::Function> CreateTestFunction(
    const std::string& name, const ngraph::PartialShape& ps) {
    using namespace ngraph;
    const auto param = std::make_shared<op::Parameter>(element::f16, ps);
    const auto convert = std::make_shared<op::Convert>(param, element::f32);
    const auto result = std::make_shared<op::Result>(convert);
    return std::make_shared<Function>(ResultVector{result},
                                      ParameterVector{param}, name);
}
}  // namespace

TEST_F(SerializationCleanupTest, SerializationShouldWork) {
    const auto f =
        CreateTestFunction("StaticFunction", ngraph::PartialShape{2, 2});

    const InferenceEngine::CNNNetwork net{f};
    net.serialize(m_out_xml_path, m_out_bin_path);

    // .xml & .bin files should be present
    ASSERT_TRUE(std::ifstream(m_out_xml_path, std::ios::in).good());
    ASSERT_TRUE(std::ifstream(m_out_bin_path, std::ios::in).good());
}

TEST_F(SerializationCleanupTest, SerializationShouldWorkWithDynamicFunction) {
    const auto f =
        CreateTestFunction("DynamicFunction",
                           ngraph::PartialShape{ngraph::Dimension()});

    const InferenceEngine::CNNNetwork net{f};
    net.serialize(m_out_xml_path, m_out_bin_path);

    // .xml & .bin files should be present
    ASSERT_TRUE(std::ifstream(m_out_xml_path, std::ios::in).good());
    ASSERT_TRUE(std::ifstream(m_out_bin_path, std::ios::in).good());
}
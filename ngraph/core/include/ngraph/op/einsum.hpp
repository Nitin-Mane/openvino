// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "ngraph/node.hpp"
#include "ngraph/op/op.hpp"

namespace ngraph {
namespace op {
namespace v7 {
/// \brief Einsum operation.
class NGRAPH_API Einsum : public Op {
public:
    NGRAPH_RTTI_DECLARATION;

    Einsum() = default;

    ///
    /// \brief      Constructs Einsum operation.
    ///
    /// \param      inputs        Input nodes on which Einsum operation performs
    /// contraction
    ///
    /// \param      equation      Einstein summation convention
    ///
    Einsum(const OutputVector& inputs, const std::string& equation);

    void validate_and_infer_types() override;

    bool visit_attributes(AttributeVisitor& visitor) override;

    std::shared_ptr<Node> clone_with_new_inputs(const OutputVector& new_args) const override;

    /// \brief      Get an equation of Einsum operation
    ///
    /// \return     Einsum equation
    ///
    std::string get_equation() const {
        return m_equation;
    }

    /// \brief      Check correctness of equation format and extract input subscripts
    /// and output subscript
    ///
    /// \param      equation              Equation to be parsed and checked
    ///
    /// \param      input_subscripts      A vector of extracted input subscripts
    ///
    /// \param      output_subscript      An output subscript
    ///
    static void parse_equation(const std::string& equation,
                               std::vector<std::string>& input_subscripts,
                               std::string& output_subscript);

    /// \brief      Extract labels (from subscript) that can be alphabetic letters or
    /// ellipsis
    ///
    /// \param      subscript      Subscript
    ///
    /// \return     A vector of extracted labels from the input subscript in the order
    /// of appearence
    ///
    static std::vector<std::string> extract_labels(const std::string& subscript);

private:
    std::string m_equation;
};
}  // namespace v7
}  // namespace op
}  // namespace ngraph

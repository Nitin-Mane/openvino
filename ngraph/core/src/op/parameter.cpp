// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "ngraph/op/parameter.hpp"

#include <sstream>

#include "itt.hpp"
#include "ngraph/attribute_visitor.hpp"

using namespace std;
using namespace ngraph;

NGRAPH_RTTI_DEFINITION(op::v0::Parameter, "Parameter", 0);

op::Parameter::Parameter(const element::Type& element_type, const PartialShape& pshape)
    : m_partial_shape(pshape),
      m_element_type(element_type),
      m_is_relevant_to_shapes(false) {
    constructor_validate_and_infer_types();
}

bool op::Parameter::visit_attributes(AttributeVisitor& visitor) {
    NGRAPH_OP_SCOPE(v0_Parameter_visit_attributes);
    visitor.on_attribute("shape", m_partial_shape);
    visitor.on_attribute("element_type", m_element_type);
    return true;
}

void op::Parameter::validate_and_infer_types() {
    NGRAPH_OP_SCOPE(v0_Parameter_validate_and_infer_types);
    Op::validate_and_infer_types();
    set_output_type(0, m_element_type, m_partial_shape);
}

shared_ptr<Node> op::Parameter::clone_with_new_inputs(const OutputVector& new_args) const {
    NGRAPH_OP_SCOPE(v0_Parameter_clone_with_new_inputs);
    check_new_args_count(this, new_args);
    return make_shared<Parameter>(m_element_type, m_partial_shape);
}

bool op::Parameter::is_relevant_to_shapes() const {
    return m_is_relevant_to_shapes;
}

void op::Parameter::set_is_relevant_to_shapes(bool is_relevant) {
    m_is_relevant_to_shapes = is_relevant;
}

constexpr DiscreteTypeInfo AttributeAdapter<ParameterVector>::type_info;

AttributeAdapter<ParameterVector>::AttributeAdapter(ParameterVector& ref) : m_ref(ref) {}

bool AttributeAdapter<ParameterVector>::visit_attributes(AttributeVisitor& visitor) {
    size_t size = m_ref.size();
    visitor.on_attribute("size", size);
    if (size != m_ref.size()) {
        m_ref.resize(size);
    }
    ostringstream index;
    for (size_t i = 0; i < size; i++) {
        index.str("");
        index << i;
        string id;
        if (m_ref[i]) {
            id = visitor.get_registered_node_id(m_ref[i]);
        }
        visitor.on_attribute(index.str(), id);
        if (!m_ref[i]) {
            m_ref[i] = ov::as_type_ptr<op::v0::Parameter>(visitor.get_registered_node(id));
        }
    }
    return true;
}

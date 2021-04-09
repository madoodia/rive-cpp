#include "animation/transition_bool_condition.hpp"
#include "animation/state_machine_input_instance.hpp"
#include "animation/state_machine_bool.hpp"
#include "animation/transition_condition_op.hpp"

using namespace rive;

bool TransitionBoolCondition::validateInputType(
    const StateMachineInput* input) const
{
	// A null input is valid as the StateMachine can attempt to limp along if we
	// introduce new input types that old conditions are expected to handle in
	// newer runtimes. The older runtimes will just evaluate them to true.
	return input == nullptr || input->is<StateMachineBool>();
}

bool TransitionBoolCondition::evaluate(
    const StateMachineInputInstance* inputInstance) const
{
	if (inputInstance == nullptr)
	{
		return true;
	}
	auto boolInput =
	    reinterpret_cast<const StateMachineBoolInstance*>(inputInstance);

	return (boolInput->value() && op() == TransitionConditionOp::equal) ||
	       (!boolInput->value() && op() == TransitionConditionOp::notEqual);
}
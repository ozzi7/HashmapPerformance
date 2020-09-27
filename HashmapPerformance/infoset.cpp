#include "infoset.h"

Infoset::Infoset()
{
}
Infoset::Infoset(int actions, bool trackAverageStrategy)
{
	// chance node
	regret.resize(actions);

	if(trackAverageStrategy)
		actionCounter.resize(actions);
	hasActionCounter = trackAverageStrategy;
}
std::vector<float> Infoset::CalculateStrategy()
{
	float sum = 0;
	std::vector<float> moveProbs = std::vector<float>(regret.size(), 0.0f);
	for (int a = 0; a < regret.size(); ++a)
		sum += max_macro(0, regret[a]);

	for (int a = 0; a < regret.size(); ++a)
	{
		if (sum > 0.0f)
			moveProbs[a] = max_macro(0, regret[a]) / sum;
		else
			moveProbs[a] = 1.0f / regret.size();
	}
	return moveProbs;
}
/// <summary>
/// Final strategy only tracked in round 0!
/// </summary>
/// <returns></returns>
std::vector<float> Infoset::GetFinalStrategy()
{
	if(!hasActionCounter)
		return std::vector<float>(regret.size(), 1.0f/regret.size());
	else
	{
		float sum = std::accumulate(actionCounter.begin(), actionCounter.end(), 0.0f);

		if (sum > 0.0f)
		{
			std::vector<float> moveProbs = std::vector<float>(actionCounter.size(), 0.0f);
			for (int a = 0; a < actionCounter.size(); ++a)
				moveProbs[a] = actionCounter[a] / sum;

			return moveProbs;
		}
		else
			return std::vector<float>(actionCounter.size(), 1.0f/actionCounter.size());
	}
}
Infoset::~Infoset() {}

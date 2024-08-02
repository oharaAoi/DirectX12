#include "AnimationUtils.h"

aiVector3D LinearInterpolation(const aiVector3D& start, const aiVector3D& end, float factor) {
	return start + factor * (end - start);
}

aiVector3D InterpolationPosition(const NodeAnimation& nodeAnim, float time) {
	// キーフレームが存在しない時
	if (nodeAnim.numPositionKeys == 0) {
		return aiVector3D(); // (0,0,0)を返す
	}

	// キーフレームが1つの場合
	if (nodeAnim.numPositionKeys == 1) {
		return nodeAnim.positionData.vectorKeys[0].mValue;
	}

	// キーフレーム間で補完するためのインデックスを見つける
	uint32_t keyIndex = 0;
	for (uint32_t oi = 0; oi < nodeAnim.numPositionKeys - 1; ++oi) {
		if (time < static_cast<float>(nodeAnim.positionData.vectorKeys[oi + 1].mTime)) {
			keyIndex = oi;
			break;
		}
	}

	// キーフレームを取得
	const aiVectorKey& startKey = nodeAnim.positionData.vectorKeys[keyIndex];
	const aiVectorKey& endKey = nodeAnim.positionData.vectorKeys[keyIndex + 1];

	// 補間係数を計算
	float duration = static_cast<float>(endKey.mTime - startKey.mTime);
	if (duration <= 0.0f) {
		return startKey.mValue; // 始点と終点が同じ時間の場合
	}
	float factor = (time - float(startKey.mTime)) / duration;

	// 補完を行う
	return LinearInterpolation(startKey.mValue, endKey.mValue, factor);
}

aiVector3D InterpolationRotation(const NodeAnimation& nodeAnim, float time) {
	// キーフレームが存在しない時
	if (nodeAnim.numRotationKeys == 0) {
		return aiVector3D(); // (0,0,0)を返す
	}

	// キーフレームが1つの場合
	if (nodeAnim.numRotationKeys == 1) {
		//return nodeAnim.rotationData.quatKeys[0].mValue;
		return aiVector3D();
	}

	// キーフレーム間で補完するためのインデックスを見つける
	uint32_t keyIndex = 0;
	for (uint32_t oi = 0; oi < nodeAnim.numRotationKeys - 1; ++oi) {
		if (time < static_cast<float>(nodeAnim.rotationData.vectorKeys[oi + 1].mTime)) {
			keyIndex = oi;
			break;
		}
	}

	// キーフレームを取得
	const aiVectorKey& startKey = nodeAnim.rotationData.vectorKeys[keyIndex];
	const aiVectorKey& endKey = nodeAnim.rotationData.vectorKeys[keyIndex + 1];

	// 補間係数を計算
	float duration = static_cast<float>(endKey.mTime - startKey.mTime);
	if (duration <= 0.0f) {
		return startKey.mValue; // 始点と終点が同じ時間の場合
	}
	float factor = (time - float(startKey.mTime)) / duration;

	// 補完を行う
	return LinearInterpolation(startKey.mValue, endKey.mValue, factor);
}

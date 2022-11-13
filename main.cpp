#include <Novice.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include <assert.h>

const char kWindowTitle[] = "LC1A_22_フミモトコウサク_タイトル";

typedef struct Vector2 {
	float x;
	float y;
};

typedef struct Matrix3x3 {
	float m[3][3];
};

Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2) {
	Matrix3x3 result;
	result.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] +
		matrix1.m[0][2] * matrix2.m[2][0];
	result.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] +
		matrix1.m[0][2] * matrix2.m[2][1];
	result.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] +
		matrix1.m[0][2] * matrix2.m[2][2];

	result.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] +
		matrix1.m[1][2] * matrix2.m[2][0];
	result.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] +
		matrix1.m[1][2] * matrix2.m[2][1];
	result.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] +
		matrix1.m[1][2] * matrix2.m[2][2];

	result.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] +
		matrix1.m[2][2] * matrix2.m[2][0];
	result.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] +
		matrix1.m[2][2] * matrix2.m[2][1];
	result.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] +
		matrix1.m[2][2] * matrix2.m[2][2];
	return result;
}

Matrix3x3 MakeRotateMatrix(float theta) {
	Matrix3x3 result;
	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[0][2] = 0.0f;
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[1][2] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	return result;
}

Matrix3x3 MakeTranslateMatrix(Vector2 translate) {
	Matrix3x3 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1.0f;
	return result;
}

Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

Vector2 ToScreen(const Vector2* world) {
	const Vector2 kWorldToScreenTranslate = { 0.0f,500.0f };

	const Vector2 kWorldToScreenScale = { 1.0f,-1.0f };

	return
	{
		(world->x * kWorldToScreenScale.x) + kWorldToScreenTranslate.x,
		(world->y * kWorldToScreenScale.y) + kWorldToScreenTranslate.y
	};
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	int TextureHandle = Novice::LoadTexture("white1x1.png");

	const Vector2 kRectsize = { 200,100 };

	const Vector2 LeftTop = {
		-kRectsize.x / 2.0f,
		kRectsize.y / 2.0f
	};

	const Vector2 RightTop = {
		kRectsize.x / 2.0f,
		kRectsize.y / 2.0f
	};

	const Vector2 LeftBottom = {
		-kRectsize.x / 2.0f,
		-kRectsize.y / 2.0f
	};

	const Vector2 RightBottom = {
		kRectsize.x / 2.0f,
		-kRectsize.y / 2.0f
	};

	Vector2 rectCenter = { 400,100 };
	float theta = 0.0f;

	const int Kspeed = 4;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		theta += M_PI / 120.0f;

		if (keys[DIK_W] != 0)
		{
			rectCenter.y += Kspeed;
		}

		if (keys[DIK_S] != 0)
		{
			rectCenter.y -= Kspeed;
		}

		if (keys[DIK_A] != 0)
		{
			rectCenter.x -= Kspeed;
		}

		if (keys[DIK_D] != 0)
		{
			rectCenter.x += Kspeed;
		}


		Matrix3x3 rotateMatrix = MakeRotateMatrix(theta);
		Matrix3x3 translateMatrix = MakeTranslateMatrix(rectCenter);
		Matrix3x3 worldMatrix = Multiply(rotateMatrix, translateMatrix);
		Vector2 leftTop = Transform(LeftTop, worldMatrix);
		Vector2 rightTop = Transform(RightTop, worldMatrix);
		Vector2 leftBottom = Transform(LeftBottom, worldMatrix);
		Vector2 rightBottom = Transform(RightBottom, worldMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		leftTop = ToScreen(&leftTop);
		rightTop = ToScreen(&rightTop);
		leftBottom = ToScreen(&leftBottom);
		rightBottom = ToScreen(&rightBottom);


		Novice::DrawQuad(int(leftTop.x), int(leftTop.y), int(rightTop.x), int(rightTop.y),
			int(leftBottom.x), int(leftBottom.y), int(rightBottom.x), int(rightBottom.y),
			0, 0, 1, 1, TextureHandle, WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

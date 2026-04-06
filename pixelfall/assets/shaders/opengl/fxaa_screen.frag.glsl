#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform vec2 rcpFrame; // 1.0 / textureSize — one pixel step in UV space

#define FXAA_SPAN_MAX   4.0
#define FXAA_REDUCE_MUL (1.0 / 8.0)
#define FXAA_REDUCE_MIN (1.0 / 128.0)

void main()
{
    // Sample the four diagonal neighbors and the center pixel
    vec3 rgbNW = texture(screenTexture, TexCoord + vec2(-rcpFrame.x, -rcpFrame.y)).rgb;
    vec3 rgbNE = texture(screenTexture, TexCoord + vec2( rcpFrame.x, -rcpFrame.y)).rgb;
    vec3 rgbSW = texture(screenTexture, TexCoord + vec2(-rcpFrame.x,  rcpFrame.y)).rgb;
    vec3 rgbSE = texture(screenTexture, TexCoord + vec2( rcpFrame.x,  rcpFrame.y)).rgb;
    vec3 rgbM  = texture(screenTexture, TexCoord).rgb;

    // Convert to luma (perceived brightness) to detect edges
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    // Compute the blur direction along the edge
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * rcpDirMin)) * rcpFrame;

    // Two blended samples along the edge direction
    vec3 rgbA = 0.5 * (
        texture(screenTexture, TexCoord + dir * (1.0/3.0 - 0.5)).rgb +
        texture(screenTexture, TexCoord + dir * (2.0/3.0 - 0.5)).rgb);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(screenTexture, TexCoord + dir * -0.5).rgb +
        texture(screenTexture, TexCoord + dir *  0.5).rgb);

    // Use the wider sample only if it stays within the local luma range
    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        FragColor = vec4(rgbA, 1.0);
    else
        FragColor = vec4(rgbB, 1.0);
}

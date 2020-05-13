#include "BackgroundImage.h"

#include "imgui/imgui.h"
#include "confparse.h"

#include <filesystem>

BackgroundImage::BackgroundImage(const Side &side) : side(&side) {
}

BackgroundImage::BackgroundImage(const int &side) : side(reinterpret_cast<const Side*>(&side)) {
}

void BackgroundImage::loadFromConfig(const std::string &filename) {
	configFilename = filename; // save filename for latter use with writeToConfig

	auto configDir = std::filesystem::path{filename}.parent_path();

	auto confparse = Confparse{};
	confparse.Load(filename);

	auto topImageFilename = confparse.ParseStr("TopImageFile", nullptr);
	if (topImageFilename == nullptr) { //No image in config, clear current image
		topImage = {};
	} else {
		topImage = Image{configDir/topImageFilename};
		topImage.offsetX = confparse.ParseInt("TopImageOffsetX", 0);
		topImage.offsetY = confparse.ParseInt("TopImageOffsetY", 0);
		topImage.scalingX = confparse.ParseDouble("TopImageScalingX", 1.0);
		topImage.scalingY = confparse.ParseDouble("TopImageScalingY", 1.0);
		topImage.mirrorX = confparse.ParseBool("TopImageMirrorX", false);
		topImage.mirrorY = confparse.ParseBool("TopImageMirrorY", false);
		topImage.transparency = confparse.ParseDouble("TopImageTransparency", 0.0);
	}

	auto bottomImageFilename = confparse.ParseStr("BottomImageFile", nullptr);
	if (bottomImageFilename == nullptr) { //No image in config, clear current image
		bottomImage = {};
	} else {
		bottomImage = Image{configDir/bottomImageFilename};
		bottomImage.offsetX = confparse.ParseInt("BottomImageOffsetX", 0);
		bottomImage.offsetY = confparse.ParseInt("BottomImageOffsetY", 0);
		bottomImage.scalingX = confparse.ParseDouble("BottomImageScalingX", 1.0);
		bottomImage.scalingY = confparse.ParseDouble("BottomImageScalingY", 1.0);
		bottomImage.mirrorX = confparse.ParseBool("BottomImageMirrorX", false);
		bottomImage.mirrorY = confparse.ParseBool("BottomImageMirrorY", false);
		bottomImage.transparency = confparse.ParseDouble("BottomImageTransparency", 0.0);
	}
	writeToConfig(filename);
	reload();
}

void BackgroundImage::writeToConfig(const std::string &filename) {
	if (filename.empty()) // No destination file to save to
		return;

	auto confparse = Confparse{};
	confparse.Load(filename);

	auto configDir = std::filesystem::path{filename}.parent_path();

	auto topImagePath = std::filesystem::relative(topImage.file, configDir);
	confparse.WriteStr("TopImageFile", topImagePath.generic_string().c_str());
	confparse.WriteInt("TopImageOffsetX", topImage.offsetX);
	confparse.WriteInt("TopImageOffsetY", topImage.offsetY);
	confparse.WriteFloat("TopImageScalingX", topImage.scalingX);
	confparse.WriteFloat("TopImageScalingY", topImage.scalingY);
	confparse.WriteBool("TopImageMirrorX", topImage.mirrorX);
	confparse.WriteBool("TopImageMirrorY", topImage.mirrorY);
	confparse.WriteFloat("TopImageTransparency", topImage.transparency);

	auto bottomImagePath = std::filesystem::relative(bottomImage.file, configDir);
	confparse.WriteStr("BottomImageFile", bottomImagePath.generic_string().c_str());
	confparse.WriteInt("BottomImageOffsetX", bottomImage.offsetX);
	confparse.WriteInt("BottomImageOffsetY", bottomImage.offsetY);
	confparse.WriteFloat("BottomImageScalingX", bottomImage.scalingX);
	confparse.WriteFloat("BottomImageScalingY", bottomImage.scalingY);
	confparse.WriteBool("BottomImageMirrorX", bottomImage.mirrorX);
	confparse.WriteBool("BottomImageMirrorY", bottomImage.mirrorY);
	confparse.WriteFloat("BottomImageTransparency", bottomImage.transparency);
}

bool BackgroundImage::reload() {
	auto ret = topImage.reload();
	ret &= bottomImage.reload();
	return ret;
}

const Image &BackgroundImage::selectedImage() const {
	switch (*side) {
		case Side::TOP:
			return topImage;
		case Side::BOTTOM:
		default:
			return bottomImage;
	}
}

void BackgroundImage::render(ImDrawList &draw, const ImVec2 &p_min, const ImVec2 &p_max, int rotation) const {
	selectedImage().render(draw, p_min, p_max, rotation);
}

float BackgroundImage::x0() const {
	return selectedImage().x0();
}

float BackgroundImage::y0() const {
	return selectedImage().y0();
}

float BackgroundImage::x1() const {
	return selectedImage().x1();
}

float BackgroundImage::y1() const {
	return selectedImage().y1();
}


#include "rive/math/hit_test.hpp"
#include "rive/shapes/image.hpp"
#include "rive/backboard.hpp"
#include "rive/importers/backboard_importer.hpp"
#include "rive/assets/file_asset.hpp"
#include "rive/assets/image_asset.hpp"
#include "rive/shapes/mesh.hpp"
#include "rive/artboard.hpp"

using namespace rive;

void Image::draw(Renderer* renderer)
{
    rive::RenderImage* renderImage;
    rive::ImageAsset* asset = this->imageAsset();
    if (asset == nullptr || renderOpacity() == 0.0f ||
        (renderImage = asset->renderImage()) == nullptr)
    {
        return;
    }

    if (!clip(renderer))
    {
        // We didn't clip, so make sure to save as we'll be doing some
        // transformations.
        renderer->save();
    }

    auto width = renderImage->width();
    auto height = renderImage->height();

    if (m_Mesh != nullptr)
    {
        m_Mesh->draw(renderer, renderImage, blendMode(), renderOpacity());
    }
    else
    {
        renderer->transform(worldTransform());
        renderer->translate(-width * originX(), -height * originY());
        renderer->drawImage(renderImage, blendMode(), renderOpacity());
    }

    renderer->restore();
}

Core* Image::hitTest(HitInfo* hinfo, const Mat2D& xform)
{
    // TODO: handle clip?

    auto renderImage = imageAsset()->renderImage();
    int width = renderImage->width();
    int height = renderImage->height();

    if (m_Mesh)
    {
        printf("Missing mesh\n");
        // TODO: hittest mesh
    }
    else
    {
        auto mx = xform * worldTransform() *
                  Mat2D::fromTranslate(-width * originX(), -height * originY());
        HitTester tester(hinfo->area);
        tester.addRect(AABB(0, 0, (float)width, (float)height), mx);
        if (tester.test())
        {
            return this;
        }
    }
    return nullptr;
}

StatusCode Image::import(ImportStack& importStack)
{
    auto result = registerReferencer(importStack);
    if (result != StatusCode::Ok)
    {
        return result;
    }
    return Super::import(importStack);
}

// Question: thoughts on this? it looks a bit odd to me,
// maybe there's a trick i'm missing here .. (could also implement getAssetId...)
uint32_t Image::assetId() { return ImageBase::assetId(); }

void Image::setAsset(FileAsset* asset)
{
    if (asset->is<ImageAsset>())
    {
        FileAssetReferencer::setAsset(asset);

        // If we have a mesh and we're in the source artboard, let's initialize
        // the mesh buffers.
        if (m_Mesh != nullptr && !artboard()->isInstance())
        {
            m_Mesh->initializeSharedBuffers(imageAsset()->renderImage());
        }
    }
}

Core* Image::clone() const
{
    Image* twin = ImageBase::clone()->as<Image>();
    if (m_fileAsset != nullptr)
    {
        twin->setAsset(m_fileAsset);
    }
    return twin;
}

void Image::setMesh(Mesh* mesh) { m_Mesh = mesh; }
Mesh* Image::mesh() const { return m_Mesh; }

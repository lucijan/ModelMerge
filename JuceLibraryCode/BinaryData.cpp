/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== mono.svg ==================
static const unsigned char temp_binary_data_0[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<svg width=\"24px\" height=\"24px\" viewBox=\"0 0 24 24\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
"    <!-- Generator: Sketch 55.2 (78181) - https://sketchapp.com -->\n"
"    <title>Slice 1</title>\n"
"    <desc>Created with Sketch.</desc>\n"
"    <g id=\"Page-1\" stroke=\"none\" stroke-width=\"1\" fill=\"none\" fill-rule=\"evenodd\">\n"
"        <circle id=\"Oval\" stroke=\"#000000\" cx=\"12\" cy=\"12\" r=\"7\"></circle>\n"
"        <rect id=\"Rectangle\" x=\"0\" y=\"0\" width=\"24\" height=\"24\"></rect>\n"
"    </g>\n"
"</svg>";

const char* mono_svg = (const char*) temp_binary_data_0;

//================== stereo.svg ==================
static const unsigned char temp_binary_data_1[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<svg width=\"24px\" height=\"24px\" viewBox=\"0 0 24 24\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
"    <!-- Generator: Sketch 55.2 (78181) - https://sketchapp.com -->\n"
"    <title>Slice 1</title>\n"
"    <desc>Created with Sketch.</desc>\n"
"    <g id=\"Page-1\" stroke=\"none\" stroke-width=\"1\" fill=\"none\" fill-rule=\"evenodd\">\n"
"        <g id=\"stereo\" transform=\"translate(2.000000, 5.000000)\" stroke=\"#000000\">\n"
"            <circle id=\"Oval\" cx=\"7\" cy=\"7\" r=\"7\"></circle>\n"
"            <circle id=\"Oval-Copy\" cx=\"13\" cy=\"7\" r=\"7\"></circle>\n"
"        </g>\n"
"        <rect id=\"Rectangle\" x=\"0\" y=\"0\" width=\"24\" height=\"24\"></rect>\n"
"    </g>\n"
"</svg>";

const char* stereo_svg = (const char*) temp_binary_data_1;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xecb1ff08:  numBytes = 562; return mono_svg;
        case 0x28ffc99d:  numBytes = 709; return stereo_svg;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "mono_svg",
    "stereo_svg"
};

const char* originalFilenames[] =
{
    "mono.svg",
    "stereo.svg"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}


#include "Internationalizator.h"

#ifdef _WIN32
#pragma warning(disable:4503)
#pragma warning(push)
#pragma warning(disable:4996 4251 4275 4800)
#endif
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#ifdef _WIN32
#pragma warning(pop)
#endif

using namespace std;
using boost::recursive_mutex;
using boost::lock_guard;

namespace charliesoft
{
  recursive_mutex _internationalizatorMutex;
  Internationalizator *Internationalizator::ptr = NULL;

  boost::format my_format(const std::string & f_string) {
    using namespace boost::io;
    boost::format fmter(f_string);
    fmter.exceptions(no_error_bits);//no exceptions wanted!
    return fmter;
  }

  Internationalizator::Internationalizator()
  {
    initTranslations();
  }

  Internationalizator* Internationalizator::getInstance()
  {
    lock_guard<recursive_mutex> guard(_internationalizatorMutex);
    if (ptr == NULL)
      ptr = new Internationalizator();
    return ptr;
  };

  void Internationalizator::releaseInstance()
  {
    lock_guard<recursive_mutex> guard(_internationalizatorMutex);
    if (ptr != NULL)
      delete ptr;
    ptr = NULL;
  };

  void Internationalizator::setLang(std::string resourceFile)
  {
    ///\todo
  };

  std::string Internationalizator::getTranslation(std::string key){
    if (translations.find(key) != translations.end())
      return translations[key];
    else
      return key;
  };


  std::string _STR(std::string key)
  {
    return Internationalizator::getInstance()->getTranslation(key).c_str();
  };

  QString _QT(std::string key)
  {
    QString output = Internationalizator::getInstance()->getTranslation(key).c_str();
    return output;
  };

  void Internationalizator::initTranslations()
  {
    translations["TRUE"] = "True";
    translations["FALSE"] = "False";
    translations["ALL_TYPES"] = "All types";

    translations["BUTTON_OK"] = "OK";
    translations["BUTTON_CANCEL"] = "Cancel";
    translations["BUTTON_DELETE"] = "Delete";
    translations["BUTTON_BROWSE"] = "Browse...";
    translations["BUTTON_UPDATE"] = "Update";
    translations["BUTTON_COLOR"] = "Color editor";
    translations["BUTTON_MATRIX"] = "Matrix editor";
    translations["BUTTON_ADD_INPUT"] = "Add input...";
    translations["BUTTON_ADD_OUTPUT"] = "Add output...";
    translations["BUTTON_SWITCH_SYNC"] = "Change block rendering type<br/>(currently synchrone)";
    translations["BUTTON_SWITCH_ASYNC"] = "Change block rendering type<br/>(currently asyncrone)";
    translations["BUTTON_SWITCH_ONESHOT"] = "Change block rendering type<br/>(currently one shot)";

    translations["TYPE_DATAS_BOOL"] = "Boolean";
    translations["TYPE_DATAS_INT"] = "Int";
    translations["TYPE_DATAS_FLOAT"] = "Float";
    translations["TYPE_DATAS_COLOR"] = "Color";
    translations["TYPE_DATAS_MATRIX"] = "Matrix";
    translations["TYPE_DATAS_STRING"] = "String";
    translations["TYPE_DATAS_FILE"] = "FilePath";
    translations["TYPE_DATAS_LISTBOX"] = "ListBox";
    translations["TYPE_DATAS_ERROR"] = "typeError";

    translations["CONDITION_EDITOR"] = "Condition editor...";
    translations["CONDITION_EDITOR_HELP"] = "You can define here which conditions are needed for block rendering!";
    translations["CONDITION_BLOCK_ERROR_INPUT"] = "Condition can't be input of block...";
    translations["CONDITION_BLOCK_LEFT"] = "left";
    translations["CONDITION_BLOCK_HELP"] = "link to the output's block<br/>whose value will be used in condition";
    translations["CONDITION_BLOCK_RIGHT"] = "right";
    translations["CONDITION_CARDINAL"] = "#rendering";
    translations["CONDITION_IS_EMPTY"] = "is empty";

    translations["NOT_INITIALIZED"] = "Not initialized...";
    translations["PROCESSING_TIME"] = "Mean processing time: ";

    translations["BLOCK_OUTPUT"] = "output";
    translations["BLOCK_INPUT"] = "input";
    translations["BLOCK_TITLE_INPUT"] = "Input";
    translations["BLOCK_TITLE_IMG_PROCESS"] = "2D processing";
    translations["BLOCK_TITLE_SIGNAL"] = "Video processing";
    translations["BLOCK_TITLE_MATH"] = "Math op.";
    translations["BLOCK_TITLE_OUTPUT"] = "Output";
    translations["BLOCK_TITLE_INFOS"] = "Infos";
    translations["BLOCK_INFOS"] =
      "<h1>Statistics</h1>"
      "<table><tr><td>Mean processing time:</td><td>%1$s ms</td></tr>"
      "<tr><td>Max processing time:</td><td>%2$s ms</td></tr>"
      "<tr><td>Min processing time:</td><td>%3$s ms</td></tr>"
      "<tr><td>Nb rendering:</td><td>%4$s</td></tr></table>"
      "<h1>Errors</h1>"
      "<p>%5$s</p>";

    translations["ERROR_GENERIC"] = "Error undefined!";
    translations["ERROR_GENERIC_TITLE"] = "Error!";
    translations["ERROR_CONFIRM_SET_VALUE"] = "Wrong input, are you sure you want to set this value?";
    translations["ERROR_TYPE"] = "The type of \"%1$s.%2$s\" (%3$s) doesn't correspond to \"%4$s.%5$s\" (%6$s)";
    translations["ERROR_LINK_WRONG_INPUT_OUTPUT"] = "You can't link %1$s to %2$s : same type (%3$s)!";
    translations["ERROR_LINK_SAME_BLOCK"] = "You can't link the same block!";
    translations["ERROR_PARAM_EXCLUSIF"] = "Params \"%1$s\" and \"%2$s\" are mutually exclusive...";
    translations["ERROR_PARAM_NEEDED"] = "Param \"%1$s\" is required...";
    translations["ERROR_PARAM_ONLY_POSITIF"] = "Param \"%1$s\":<br/>only positive value are authorized!";
    translations["ERROR_PARAM_ONLY_POSITIF_STRICT"] = "Param \"%1$s\":<br/>only strict positive value are authorized!";
    translations["ERROR_PARAM_ONLY_NEGATIF"] = "Param \"%1$s\":<br/>only negative value are authorized!";
    translations["ERROR_PARAM_ONLY_NEGATIF_STRICT"] = "Param \"%1$s\":<br/>only strict negative value are authorized!";
    translations["ERROR_PARAM__valueBETWEEN"] = "Param \"%1$s\" (%2$f):<br/>should be between %3$f and %4$f";
    translations["ERROR_PARAM_FOUND"] = "Error! Could not find property of \"%1$s\"";

    translations["MENU_FILE"] = "File";
    translations["MENU_FILE_OPEN"] = "Open";
    translations["MENU_FILE_OPEN_TIP"] = "Open a previous project";
    translations["MENU_FILE_CREATE"] = "New";
    translations["MENU_FILE_CREATE_TIP"] = "Create a new project";
    translations["MENU_FILE_SAVE"] = "Save";
    translations["MENU_FILE_SAVE_TIP"] = "Save current project";
    translations["MENU_FILE_SAVEAS"] = "Save as...";
    translations["MENU_FILE_SAVEAS_TIP"] = "Choose file where to save current project";
    translations["MENU_FILE_QUIT"] = "Quit";
    translations["MENU_FILE_QUIT_TIP"] = "Quit application";

    translations["MENU_EDIT"] = "Edit";
    translations["MENU_EDIT_SUBGRAPH"] = "Create subprocess";
    translations["MENU_EDIT_SUBGRAPH_TIP"] = "Create a subprocess using every selected widgets";

    translations["CREATE_PARAM_TITLE"] = "Parameter creation";
    translations["CREATE_PARAM_TYPE"] = "Type of the parameter: ";
    translations["CREATE_PARAM_NAME"] = "Short name of the parameter: ";
    translations["CREATE_PARAM_NAME_HELP"] = "Short description of the parameter: ";
    translations["CREATE_PARAM_INIT_VAL"] = "Initial value of the parameter: ";

    translations["MATRIX_EDITOR_TOOLS"] = "Tools";
    translations["MATRIX_EDITOR_BLOCKS"] = "Blocks";
    translations["MATRIX_EDITOR_DATA_CHOICES"] = "Matrix data type:";
    translations["MATRIX_EDITOR_DATA_SIZE"] = "Size (rows, cols, channels):";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL"] = "Initial values:";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL_0"] = "zeros";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL_1"] = "constant";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL_2"] = "eye";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL_3"] = "ellipse";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL_4"] = "rect";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL_5"] = "cross";
    translations["MATRIX_EDITOR_DATA_INITIAL_VAL_6"] = "random";
    translations["MATRIX_EDITOR_SECTION_PEN_COLOR"] = "Color:";
    translations["MATRIX_EDITOR_SECTION_PEN_SIZE"] = "Pencil size:";

    translations["MATRIX_EDITOR_HELP_LEFT"] = "Panning left (CTRL+arrowLEFT)";
    translations["MATRIX_EDITOR_HELP_RIGHT"] = "Panning right (CTRL+arrowRIGHT)";
    translations["MATRIX_EDITOR_HELP_UP"] = "Panning up (CTRL+arrowUP)";
    translations["MATRIX_EDITOR_HELP_DOWN"] = "Panning down (CTRL+arrowDOWN)";
    translations["MATRIX_EDITOR_HELP_ZOOM_X1"] = "Zoom x1 (CTRL+P)";
    translations["MATRIX_EDITOR_HELP_ZOOM_IN"] = "Zoom in (CTRL++)";
    translations["MATRIX_EDITOR_HELP_ZOOM_OUT"] = "Zoom out (CTRL+-)";
    translations["MATRIX_EDITOR_HELP_SAVE"] = "Save current matrix (CTRL+S)";
    translations["MATRIX_EDITOR_HELP_LOAD"] = "Load new matrix (CTRL+O)";
    translations["MATRIX_EDITOR_HELP_EDIT"] = "Edit matrix (CTRL+E)";
    translations["MATRIX_EDITOR_HELP_ONTOP"] = "Always on top (CTRL+T)";
    translations["MATRIX_EDITOR_HELP_START"] = "Run graph (Enter)";
    translations["MATRIX_EDITOR_HELP_STOP"] = "Stop graph (End)";
    translations["MATRIX_EDITOR_HELP_PAUSE"] = "Pause graph (Space)";

    translations["MENU_HELP_INFO"] = "Info";
    translations["MENU_HELP_HELP"] = "Help";

    translations["CONF_FILE_TYPE"] = "imGraph project (*.igp)";

    translations["PROJ_LOAD_FILE"] = "Open project file";
    translations["PROJ_CREATE_FILE"] = "Create project file";

    translations["DOCK_TITLE"] = "Toolbox";
    translations["DOCK_PROPERTY_TITLE"] = "Properties";

    translations["SUBBLOCK__"] = "Sub graph";

    translations["FOR_BLOCK_"] = "for";
    translations["FOR_BLOCK_INITVAL"] = "start";
    translations["FOR_BLOCK_INITVAL_HELP"] = "Initial value of counter";
    translations["FOR_BLOCK_ENDVAL"] = "end";
    translations["FOR_BLOCK_ENDVAL_HELP"] = "Final value of counter";
    translations["FOR_BLOCK_STEPVAL"] = "step";
    translations["FOR_BLOCK_STEPVAL_HELP"] = "Step value of counter";

    translations["BLOCK__INPUT_NAME"] = "File Loader";
    translations["BLOCK__INPUT_IN_INPUT_TYPE"] = "input";
    translations["BLOCK__INPUT_IN_INPUT_TYPE_HELP"] = "Input type|Webcam^Video file^Folder";
    translations["BLOCK__INPUT_IN_FILE_HELP"] = "File to load.";
    translations["BLOCK__INPUT_IN_FILE_FILTER"] = "media files";
    translations["BLOCK__INPUT_IN_FILE_NOT_FOUND"] = "File \"%1$s\" not found!";
    translations["BLOCK__INPUT_IN_FILE_NOT_FOLDER"] = "File \"%1$s\" is not a folder!";
    translations["BLOCK__INPUT_IN_FILE_PROBLEM"] = "File \"%1$s\" can't be loaded!";
    translations["BLOCK__INPUT_IN_LOOP"] = "loop";
    translations["BLOCK__INPUT_IN_LOOP_HELP"] = "Loop video file (if <=0, infinite loop)";
    translations["BLOCK__INPUT_IN_GREY"] = "grey";
    translations["BLOCK__INPUT_IN_GREY_HELP"] = "Convert image to a grayscale one";
    translations["BLOCK__INPUT_IN_COLOR"] = "color";
    translations["BLOCK__INPUT_IN_COLOR_HELP"] = "Convert image to a color one";
    translations["BLOCK__INPUT_OUT_IMAGE"] = "image";
    translations["BLOCK__INPUT_OUT_IMAGE_HELP"] = "Output image";
    translations["BLOCK__INPUT_OUT_FRAMERATE"] = "framerate";
    translations["BLOCK__INPUT_OUT_FRAMERATE_HELP"] = "Number of frames per second";
    translations["BLOCK__INPUT_INOUT_WIDTH"] = "width";
    translations["BLOCK__INPUT_INOUT_WIDTH_HELP"] = "Wanted width of images (in pixels)";
    translations["BLOCK__INPUT_INOUT_HEIGHT"] = "height";
    translations["BLOCK__INPUT_INOUT_HEIGHT_HELP"] = "Wanted height of images (in pixels)";
    translations["BLOCK__INPUT_INOUT_POS_FRAMES"] = "position";
    translations["BLOCK__INPUT_INOUT_POS_FRAMES_HELP"] = "0-based index of the frame to be decoded/captured";
    translations["BLOCK__INPUT_INOUT_POS_RATIO"] = "pos. ratio";
    translations["BLOCK__INPUT_INOUT_POS_RATIO_HELP"] = "Relative position in the video file (0-begining, 1-end)";
    translations["BLOCK__INPUT_OUT_FORMAT"] = "out format";
    translations["BLOCK__INPUT_OUT_FORMAT_HELP"] = "The format of the Mat objects";

    translations["BLOCK__OUTPUT_NAME"] = "Display image";
    translations["BLOCK__OUTPUT_IN_IMAGE"] = "image";
    translations["BLOCK__OUTPUT_IN_IMAGE_HELP"] = "Image to show";
    translations["BLOCK__OUTPUT_IN_WIN_NAME"] = "win. title";
    translations["BLOCK__OUTPUT_IN_WIN_NAME_HELP"] = "Windows title";
    translations["BLOCK__OUTPUT_IN_NORMALIZE"] = "normalize";
    translations["BLOCK__OUTPUT_IN_NORMALIZE_HELP"] = "Normalize image before show";

    translations["BLOCK__SHOWGRAPH_NAME"] = "Show graph";
    translations["BLOCK__SHOWGRAPH_IN_VALUES"] = "values";
    translations["BLOCK__SHOWGRAPH_IN_VALUES_HELP"] = "Vector of values to show";
    translations["BLOCK__SHOWGRAPH_IN_TITLE"] = "title";
    translations["BLOCK__SHOWGRAPH_IN_TITLE_HELP"] = "Graph title";

    translations["BLOCK__STRING_CREATION_NAME"] = "String creation";
    translations["BLOCK__STRING_CREATION_IN_REGEX"] = "pattern";
    translations["BLOCK__STRING_CREATION_IN_REGEX_HELP"] = "Pattern name <br/>(use %i% for input i : <br/>\"img%1%.jpg\"<br/> will concatenate first input<br/> with the pattern...)<br/>%n% can also be used:<br/>it's the number of current frame";
    translations["BLOCK__STRING_CREATION_OUT"] = "output";
    translations["BLOCK__STRING_CREATION_OUT_HELP"] = "Constructed string";

    translations["BLOCK__WRITE_NAME"] = "Write video";
    translations["BLOCK__WRITE_IN_IMAGE"] = "image";
    translations["BLOCK__WRITE_IN_IMAGE_HELP"] = "Image to add to video file";
    translations["BLOCK__WRITE_IN_FILENAME"] = "Filename";
    translations["BLOCK__WRITE_IN_FILENAME_HELP"] = "Filename of the video file";
    translations["BLOCK__WRITE_IN_FPS"] = "FPS";
    translations["BLOCK__WRITE_IN_FPS_HELP"] = "Frames per second";
    translations["BLOCK__WRITE_IN_CODEC"] = "codec";
    translations["BLOCK__WRITE_IN_CODEC_HELP"] = "FOURCC wanted (XVID, X264...).<br/>Empty if you want no compression,<br/>-1 if you want to choose using IHM!";

    translations["BLOCK__IMWRITE_NAME"] = "Write image";
    translations["BLOCK__IMWRITE_IN_IMAGE"] = "image";
    translations["BLOCK__IMWRITE_IN_IMAGE_HELP"] = "Image to save";
    translations["BLOCK__IMWRITE_IN_FILENAME"] = "Filename";
    translations["BLOCK__IMWRITE_IN_FILENAME_HELP"] = "Filename of the file";
    translations["BLOCK__IMWRITE_IN_QUALITY"] = "Quality";
    translations["BLOCK__IMWRITE_IN_QUALITY_HELP"] = "Quality of the output img (0->highest compression, 100->highest quality)";

    translations["BLOCK__LINEDRAWER_NAME"] = "Draw lines";
    translations["BLOCK__LINEDRAWER_IN_LINES"] = "lines list";
    translations["BLOCK__LINEDRAWER_IN_LINES_HELP"] = "Input of lines (4 values per row)";
    translations["BLOCK__LINEDRAWER_IN_IMAGE"] = "image";
    translations["BLOCK__LINEDRAWER_IN_IMAGE_HELP"] = "Input image to draw on";
    translations["BLOCK__LINEDRAWER_IN_COLOR"] = "color";
    translations["BLOCK__LINEDRAWER_IN_COLOR_HELP"] = "Color of lines";
    translations["BLOCK__LINEDRAWER_IN_SIZE"] = "size";
    translations["BLOCK__LINEDRAWER_IN_SIZE_HELP"] = "Size of lines";
    translations["BLOCK__LINEDRAWER_OUT_IMAGE"] = "image";
    translations["BLOCK__LINEDRAWER_OUT_IMAGE_HELP"] = "Binary output image";

    translations["BLOCK__POINTDRAWER_NAME"] = "Draw points";
    translations["BLOCK__POINTDRAWER_IN_POINTS"] = "point list";
    translations["BLOCK__POINTDRAWER_IN_POINTS_HELP"] = "Input of points (2 values per row)";
    translations["BLOCK__POINTDRAWER_IN_IMAGE"] = "image";
    translations["BLOCK__POINTDRAWER_IN_IMAGE_HELP"] = "Input image to draw on";
    translations["BLOCK__POINTDRAWER_IN_COLOR"] = "color";
    translations["BLOCK__POINTDRAWER_IN_COLOR_HELP"] = "Color of points";
    translations["BLOCK__POINTDRAWER_IN_SIZE"] = "size";
    translations["BLOCK__POINTDRAWER_IN_SIZE_HELP"] = "Size of points";
    translations["BLOCK__POINTDRAWER_OUT_IMAGE"] = "image";
    translations["BLOCK__POINTDRAWER_OUT_IMAGE_HELP"] = "Binary output image";
    translations["BLOCK__POINTDRAWER_ERROR_POINT_SIZE"] = "Image and points have different sizes";

    translations["BLOCK__CREATEMATRIX_NAME"] = "Create new matrix";
    translations["BLOCK__CREATEMATRIX_IN_TYPE"] = "type";
    translations["BLOCK__CREATEMATRIX_IN_TYPE_HELP"] = "Wanted type|CV_8U^CV_8S^CV_16U^CV_16S^CV_32S^CV_32F^CV_64F";
    translations["BLOCK__CREATEMATRIX_IN_NBCHANNEL"] = "channels";
    translations["BLOCK__CREATEMATRIX_IN_NBCHANNEL_HELP"] = "Numbers of channels";
    translations["BLOCK__CREATEMATRIX_IN_WIDTH"] = translations["BLOCK__INPUT_INOUT_WIDTH"];
    translations["BLOCK__CREATEMATRIX_IN_WIDTH_HELP"] = translations["BLOCK__INPUT_INOUT_WIDTH_HELP"];
    translations["BLOCK__CREATEMATRIX_IN_HEIGHT"] = translations["BLOCK__INPUT_INOUT_HEIGHT"];
    translations["BLOCK__CREATEMATRIX_IN_HEIGHT_HELP"] = translations["BLOCK__INPUT_INOUT_HEIGHT_HELP"];
    translations["BLOCK__CREATEMATRIX_IN_INIT"] = "intialisation";
    translations["BLOCK__CREATEMATRIX_IN_INIT_HELP"] = "Initial values of matrix|zeros^ones^eye^ellipse^rect^cross^random uniform^random gaussian";
    translations["BLOCK__CREATEMATRIX_OUT_IMAGE"] = "image";
    translations["BLOCK__CREATEMATRIX_OUT_IMAGE_HELP"] = "Binary output image";

    translations["BLOCK__NORMALIZ_NAME"] = "Normalize image";
    translations["BLOCK__NORMALIZ_IN_IMAGE"] = "image";
    translations["BLOCK__NORMALIZ_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__NORMALIZ_OUT_IMAGE"] = "image";
    translations["BLOCK__NORMALIZ_OUT_IMAGE_HELP"] = "Normalized image";

    translations["BLOCK__OPTICFLOW_NAME"] = "Compute Optical flow";
    translations["BLOCK__OPTICFLOW_IN_IMAGE1"] = "image1";
    translations["BLOCK__OPTICFLOW_IN_IMAGE1_HELP"] = "First image";
    translations["BLOCK__OPTICFLOW_IN_IMAGE2"] = "image2";
    translations["BLOCK__OPTICFLOW_IN_IMAGE2_HELP"] = "Second image";
    translations["BLOCK__OPTICFLOW_IN_METHOD"] = "method";
    translations["BLOCK__OPTICFLOW_IN_METHOD_HELP"] = "Method|LK^Farneback^DualTVL1^DeepFlow";
    translations["BLOCK__OPTICFLOW_OUT_IMAGE"] = "flow";
    translations["BLOCK__OPTICFLOW_OUT_IMAGE_HELP"] = "Optical flow";

    translations["BLOCK__LINE_FINDER_NAME"] = "Find lines";
    translations["BLOCK__LINE_FINDER_IN_IMAGE"] = "image";
    translations["BLOCK__LINE_FINDER_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__LINE_FINDER_OUT_IMAGE"] = "lines";
    translations["BLOCK__LINE_FINDER_OUT_IMAGE_HELP"] = "List of detected lines";

    translations["BLOCK__POINT_FINDER_NAME"] = "Find points";
    translations["BLOCK__POINT_FINDER_IN_IMAGE"] = "image";
    translations["BLOCK__POINT_FINDER_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__POINT_FINDER_IN_DETECTOR"] = "detector";
    translations["BLOCK__POINT_FINDER_IN_DETECTOR_HELP"] = "Method used to detect points|BRISK^FAST^GFTT^HARRIS^MSER^ORB^SIFT^STAR^SURF^KAZE^AKAZE^SimpleBlob"; //^Grid^Dense
    translations["BLOCK__POINT_FINDER_IN_EXTRACTOR"] = "extractor";
    translations["BLOCK__POINT_FINDER_IN_EXTRACTOR_HELP"] = "Method used to compute descriptor|SIFT^SURF^ORB^BRIEF^BRISK^MSER^FREAK^KAZE^AKAZE";
    translations["BLOCK__POINT_FINDER_OUT_DESC"] = "descriptors";
    translations["BLOCK__POINT_FINDER_OUT_DESC_HELP"] = "List of corresponding descriptors";
    translations["BLOCK__POINT_FINDER_OUT_POINTS"] = "points";
    translations["BLOCK__POINT_FINDER_OUT_POINTS_HELP"] = "List of detected points";

    translations["BLOCK__POINT_MATCHER_NAME"] = "Match points";
    translations["BLOCK__POINT_MATCHER_IN_PT_DESC1"] = "desc1";
    translations["BLOCK__POINT_MATCHER_IN_PT_DESC1_HELP"] = "List of first points descriptors";
    translations["BLOCK__POINT_MATCHER_IN_PT_DESC2"] = "desc2";
    translations["BLOCK__POINT_MATCHER_IN_PT_DESC2_HELP"] = "List of second points descriptors";
    translations["BLOCK__POINT_MATCHER_IN_ALGO"] = "method";
    translations["BLOCK__POINT_MATCHER_IN_ALGO_HELP"] = "Method used to match feature vector|Brute force^FLANN";
    translations["BLOCK__POINT_MATCHER_OUT_MATCHES"] = "matches";
    translations["BLOCK__POINT_MATCHER_OUT_MATCHES_HELP"] = "List of matched points";
    translations["BLOCK__POINT_MATCHER_OUT_MATCHES_MASK"] = "mask";
    translations["BLOCK__POINT_MATCHER_OUT_MATCHES_MASK_HELP"] = "List of correct matched points";

    translations["BLOCK__DEINTERLACE_NAME"] = "Deinterlace";
    translations["BLOCK__DEINTERLACE_IN_IMAGE"] = "image";
    translations["BLOCK__DEINTERLACE_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__DEINTERLACE_IN_TYPE"] = "Deinterlacing";
    translations["BLOCK__DEINTERLACE_IN_TYPE_HELP"] = "Deinterlacing type wanted|Blend^Bob^Discard^Unfold";
    translations["BLOCK__DEINTERLACE_OUT_IMAGE"] = "image";
    translations["BLOCK__DEINTERLACE_OUT_IMAGE_HELP"] = "Deinterlaced image";

    translations["BLOCK__HISTOGRAM_NAME"] = "Histogram";
    translations["BLOCK__HISTOGRAM_IN_IMAGE"] = "image";
    translations["BLOCK__HISTOGRAM_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__HISTOGRAM_IN_BINS"] = "#bins";
    translations["BLOCK__HISTOGRAM_IN_BINS_HELP"] = "number of bins";
    translations["BLOCK__HISTOGRAM_IN_ACCUMULATE"] = "accumulate";
    translations["BLOCK__HISTOGRAM_IN_ACCUMULATE_HELP"] = "This feature enables you to compute a single histogram from several sets of arrays, or to update the histogram in time";
    translations["BLOCK__HISTOGRAM_OUT_HISTO"] = "histo";
    translations["BLOCK__HISTOGRAM_OUT_HISTO_HELP"] = "Histogram of the input image";

    translations["BLOCK__SKIP_FRAME_NAME"] = "Keep only 1 frame every N frame";
    translations["BLOCK__SKIP_FRAME_IN_IMAGE"] = "image";
    translations["BLOCK__SKIP_FRAME_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__SKIP_FRAME_IN_TYPE"] = "nb skip";
    translations["BLOCK__SKIP_FRAME_IN_TYPE_HELP"] = "Number of frames to skip";
    translations["BLOCK__SKIP_FRAME_OUT_IMAGE"] = "image";
    translations["BLOCK__SKIP_FRAME_OUT_IMAGE_HELP"] = "Interlaced image";

    translations["BLOCK__DELAY_VIDEO_NAME"] = "Delay the video of N frame";
    translations["BLOCK__DELAY_VIDEO_IN_IMAGE"] = "image";
    translations["BLOCK__DELAY_VIDEO_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__DELAY_VIDEO_IN_DELAY"] = "delay";
    translations["BLOCK__DELAY_VIDEO_IN_DELAY_HELP"] = "Number of frames to delay";
    translations["BLOCK__DELAY_VIDEO_OUT_IMAGE"] = "image";
    translations["BLOCK__DELAY_VIDEO_OUT_IMAGE_HELP"] = "N-th old image";

    translations["BLOCK__ADD_NAME"] = "Add";
    translations["BLOCK__ADD_IN_PARAM1"] = "input1";
    translations["BLOCK__ADD_IN_PARAM1_HELP"] = "First input (Matrix, number...)";
    translations["BLOCK__ADD_IN_PARAM2"] = "input2";
    translations["BLOCK__ADD_IN_PARAM2_HELP"] = "Second input (Matrix, number...)";
    translations["BLOCK__ADD_OUTPUT"] = "sum";
    translations["BLOCK__ADD_OUTPUT_HELP"] = "Output sum of the two input (concatenate in case of string)";

    translations["BLOCK__CROP_NAME"] = "Crop image";
    translations["BLOCK__CROP_IN_IMAGE"] = "image";
    translations["BLOCK__CROP_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__CROP_WIDTH"] = translations["BLOCK__INPUT_INOUT_WIDTH"];
    translations["BLOCK__CROP_WIDTH_HELP"] = translations["BLOCK__INPUT_INOUT_WIDTH_HELP"];
    translations["BLOCK__CROP_HEIGHT"] = translations["BLOCK__INPUT_INOUT_HEIGHT"];
    translations["BLOCK__CROP_HEIGHT_HELP"] = translations["BLOCK__INPUT_INOUT_HEIGHT_HELP"];
    translations["BLOCK__CROP_IN_X"] = "X";
    translations["BLOCK__CROP_IN_X_HELP"] = "Position of top-left corner (X)";
    translations["BLOCK__CROP_IN_Y"] = "Y";
    translations["BLOCK__CROP_IN_Y_HELP"] = "Position of top-left corner (Y)";
    translations["BLOCK__CROP_OUT_IMAGE"] = "image";
    translations["BLOCK__CROP_OUT_IMAGE_HELP"] = "Croped image";

    translations["BLOCK__ACCUMULATOR_NAME"] = "Accumulator";
    translations["BLOCK__ACCUMULATOR_IN_IMAGE"] = "image";
    translations["BLOCK__ACCUMULATOR_IN_IMAGE_HELP"] = "Input image to accumulate";
    translations["BLOCK__ACCUMULATOR_IN_NB_HISTORY"] = "history";
    translations["BLOCK__ACCUMULATOR_IN_NB_HISTORY_HELP"] = "Size of accumulation history";
    translations["BLOCK__ACCUMULATOR_OUT_IMAGE"] = "image";
    translations["BLOCK__ACCUMULATOR_OUT_IMAGE_HELP"] = "Accumulated image";

    translations["BLOCK__MORPHOLOGIC_NAME"] = "Morpho math";
    translations["BLOCK__MORPHOLOGIC_IN_IMAGE"] = "image";
    translations["BLOCK__MORPHOLOGIC_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__MORPHOLOGIC_ELEMENT"] = "element";
    translations["BLOCK__MORPHOLOGIC_ELEMENT_HELP"] = "Structuring element.";
    translations["BLOCK__MORPHOLOGIC_OPERATOR"] = "op";
    translations["BLOCK__MORPHOLOGIC_OPERATOR_HELP"] = "Operator|open^close^gradient^tophat^blackhat";
    translations["BLOCK__MORPHOLOGIC_ITERATIONS"] = "iterations";
    translations["BLOCK__MORPHOLOGIC_ITERATIONS_HELP"] = "Number of times erosion and dilation are applied.";
    translations["BLOCK__MORPHOLOGIC_OUT_IMAGE"] = "image";
    translations["BLOCK__MORPHOLOGIC_OUT_IMAGE_HELP"] = "Filtered image";

    translations["BLOCK__CANNY_NAME"] = "Canny";
    translations["BLOCK__CANNY_IN_IMAGE"] = "image";
    translations["BLOCK__CANNY_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__CANNY_OUT_IMAGE"] = "output";
    translations["BLOCK__CANNY_OUT_IMAGE_HELP"] = "Canny image";
    translations["BLOCK__CANNY_IN_THRESHOLD_1"] = "Threshold 1";
    translations["BLOCK__CANNY_IN_THRESHOLD_1_HELP"] = "1er threshold for the hysteresis procedure";
    translations["BLOCK__CANNY_IN_THRESHOLD_2"] = "Threshold 2";
    translations["BLOCK__CANNY_IN_THRESHOLD_2_HELP"] = "2nd threshold for the hysteresis procedure";
    translations["BLOCK__CANNY_IN_APERTURE_SIZE"] = "Aperture Size";
    translations["BLOCK__CANNY_IN_APERTURE_SIZE_HELP"] = "aperture size for the Sobel operator";
    translations["BLOCK__CANNY_IN_L2_GRADIENT"] = "L2 Gradient";
    translations["BLOCK__CANNY_IN_L2_GRADIENT_HELP"] = "Use more accurate L2 gradient";

    translations["BLOCK__INPUT_IN_INPUT_FILE"] = "Input";
    translations["BLOCK__INPUT_IN_INPUT_FILE_HELP"] = "File Path";
    translations["BLOCK__INPUT_RAW_VIDEO_NAME"] = "YUV reader";

    translations["BLOCK__OUTPUT_RAW_VIDEO_NAME"] = "YUV writer";

    translations["BLOCK__CASCADECLASSIFIER_NAME"] = "Cascade Classifier";
    translations["BLOCK__CASCADECLASSIFIER_IN_IMAGE"] = "image";
    translations["BLOCK__CASCADECLASSIFIER_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__CASCADECLASSIFIER_OUT_IMAGE"] = "mask";
    translations["BLOCK__CASCADECLASSIFIER_OUT_IMAGE_HELP"] = "Output mask";
    translations["BLOCK__CASCADECLASSIFIER_IN_CASCADE_FILE"] = "classifier";
    translations["BLOCK__CASCADECLASSIFIER_IN_CASCADE_FILE_HELP"] = "Loads a classifier from a file.";
    translations["BLOCK__CASCADECLASSIFIER_IN_SCALE_FACTOR"] = "scaleFactor";
    translations["BLOCK__CASCADECLASSIFIER_IN_SCALE_FACTOR_HELP"] = "Parameter specifying how much the image size is reduced at each image scale.";
    translations["BLOCK__CASCADECLASSIFIER_IN_MIN_NEIGHBORS"] = "minNeighbors";
    translations["BLOCK__CASCADECLASSIFIER_IN_MIN_NEIGHBORS_HELP"] = "Parameter specifying how many neighbors each candidate rectangle should have to retain it.";
    translations["BLOCK__CASCADECLASSIFIER_IN_MIN_WIDTH"] = "minWidth";
    translations["BLOCK__CASCADECLASSIFIER_IN_MIN_WIDTH_HELP"] = "Minimum possible object width. Objects smaller than that are ignored.";
    translations["BLOCK__CASCADECLASSIFIER_IN_MIN_HEIGHT"] = "minHeight";
    translations["BLOCK__CASCADECLASSIFIER_IN_MIN_HEIGHT_HELP"] = "Minimum possible object height. Objects smaller than that are ignored.";
    translations["BLOCK__CASCADECLASSIFIER_IN_MAX_WIDTH"] = "maxWidth";
    translations["BLOCK__CASCADECLASSIFIER_IN_MAX_WIDTH_HELP"] = "Maximum possible object width. Objects larger than that are ignored.";
    translations["BLOCK__CASCADECLASSIFIER_IN_MAX_HEIGHT"] = "maxHeight";
    translations["BLOCK__CASCADECLASSIFIER_IN_MAX_HEIGHT_HELP"] = "Maximum possible object height. Objects larger than that are ignored.";
    translations["BLOCK__CASCADECLASSIFIER_OUT_OBJECTS"] = "#objects";
    translations["BLOCK__CASCADECLASSIFIER_OUT_OBJECTS_HELP"] = "Number of detected objects";

    translations["BLOCK__BLUR_NAME"] = "Blurring";
    translations["BLOCK__BLUR_IN_METHOD"] = "method";
    translations["BLOCK__BLUR_IN_METHOD_HELP"] = "Blurring method|Mean^Gaussian^Median^Bilateral";
    translations["BLOCK__BLUR_IN_IMG"] = "image";
    translations["BLOCK__BLUR_IN_IMG_HELP"] = "Input image";
    translations["BLOCK__BLUR_OUT_IMAGE"] = "image";
    translations["BLOCK__BLUR_OUT_IMAGE_HELP"] = "Filtered image";

    translations["BLOCK__CONVERTMATRIX_NAME"] = "Convert matrix";
    translations["BLOCK__CONVERTMATRIX_IN_IMG"] = "image";
    translations["BLOCK__CONVERTMATRIX_IN_IMG_HELP"] = "Input image to convert";
    translations["BLOCK__CONVERTMATRIX_IN_TYPE"] = "type";
    translations["BLOCK__CONVERTMATRIX_IN_TYPE_HELP"] = "Wanted type|CV_8U^CV_8S^CV_16U^CV_16S^CV_32S^CV_32F^CV_64F";
    translations["BLOCK__CONVERTMATRIX_IN_NBCHANNEL"] = "channels";
    translations["BLOCK__CONVERTMATRIX_IN_NBCHANNEL_HELP"] = "Numbers of channels";
    translations["BLOCK__CONVERTMATRIX_OUT_IMAGE"] = "image";
    translations["BLOCK__CONVERTMATRIX_OUT_IMAGE_HELP"] = "Binary output image";

    translations["BLOCK__RESIZEMATRIX_NAME"] = "Resize matrix";
    translations["BLOCK__RESIZEMATRIX_IN_IMG"] = "image";
    translations["BLOCK__RESIZEMATRIX_IN_IMG_HELP"] = "Input image to resize";
    translations["BLOCK__RESIZEMATRIX_IN_WIDTH"] = translations["BLOCK__INPUT_INOUT_WIDTH"];
    translations["BLOCK__RESIZEMATRIX_IN_WIDTH_HELP"] = translations["BLOCK__INPUT_INOUT_WIDTH_HELP"];
    translations["BLOCK__RESIZEMATRIX_IN_HEIGHT"] = translations["BLOCK__INPUT_INOUT_HEIGHT"];
    translations["BLOCK__RESIZEMATRIX_IN_HEIGHT_HELP"] = translations["BLOCK__INPUT_INOUT_HEIGHT_HELP"];
    translations["BLOCK__RESIZEMATRIX_OUT_IMAGE"] = "image";
    translations["BLOCK__RESIZEMATRIX_OUT_IMAGE_HELP"] = "Binary output image";

    translations["BLOCK__CORE_NAME"] = "CORE filter";
    translations["BLOCK__CORE_IN_POINTS"] = "points";
    translations["BLOCK__CORE_IN_POINTS_HELP"] = "Input points to filter";
    translations["BLOCK__CORE_IN_DESC"] = "descriptors";
    translations["BLOCK__CORE_IN_DESC_HELP"] = "Input descriptors to filter";
    translations["BLOCK__CORE_IN_THRESHOLD"] = "threshold";
    translations["BLOCK__CORE_IN_THRESHOLD_HELP"] = "Threshold value (percent)";
    translations["BLOCK__CORE_IN_OPTIM_THRESHOLD"] = "optimization";
    translations["BLOCK__CORE_IN_OPTIM_THRESHOLD_HELP"] = "Threshold value of optimization (0->1, 1 for no optimization)";
    translations["BLOCK__CORE_OUT_POINTS"] = "points";
    translations["BLOCK__CORE_OUT_POINTS_HELP"] = "Filtered points";
    translations["BLOCK__CORE_OUT_DESC"] = "descriptors";
    translations["BLOCK__CORE_OUT_DESC_HELP"] = "Filtered descriptors";

    translations["BLOCK__FILTER2D_NAME"] = "Filter2D";
    translations["BLOCK__FILTER2D_IN_IMAGE"] = "image";
    translations["BLOCK__FILTER2D_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__FILTER2D_IN_KERNEL"] = "kernel";
    translations["BLOCK__FILTER2D_IN_KERNEL_HELP"] = "Convolution kernel ";
    translations["BLOCK__FILTER2D_OUT_IMAGE"] = "image";
    translations["BLOCK__FILTER2D_OUT_IMAGE_HELP"] = "Output image";

    translations["BLOCK__LAPLACIAN_NAME"] = "Laplacian";
    translations["BLOCK__LAPLACIAN_IN_IMAGE"] = "image";
    translations["BLOCK__LAPLACIAN_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__LAPLACIAN_OUT_IMAGE"] = "image";
    translations["BLOCK__LAPLACIAN_OUT_IMAGE_HELP"] = "Output image";

    translations["BLOCK__MASKDRAWER_NAME"] = "Draw mask";
    translations["BLOCK__MASKDRAWER_IN_IMAGE"] = "image";
    translations["BLOCK__MASKDRAWER_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__MASKDRAWER_IN_MASK"] = "mask";
    translations["BLOCK__MASKDRAWER_IN_MASK_HELP"] = "Input mask";
    translations["BLOCK__MASKDRAWER_IN_PRINTMASK"] = "Masking type";
    translations["BLOCK__MASKDRAWER_IN_PRINTMASK_HELP"] = "How to use the mask?|Draw mask^Keep only mask"; 
    translations["BLOCK__MASKDRAWER_OUT_IMAGE"] = "image";
    translations["BLOCK__MASKDRAWER_OUT_IMAGE_HELP"] = "Output image";
    translations["BLOCK__MASKDRAWER_ERROR_MASK_TYPE"] = "Expected mask of type CV_8UC1";
    translations["BLOCK__MASKDRAWER_ERROR_DIFF_SIZE"] = "Image and mask have different sizes";

    translations["BLOCK__THINNING_NAME"] = "Thinning img";
    translations["BLOCK__THINNING_IN_IMAGE"] = "image";
    translations["BLOCK__THINNING_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__THINNING_OUT_IMAGE"] = "image";
    translations["BLOCK__THINNING_OUT_IMAGE_HELP"] = "Output image";

    translations["BLOCK__DISTANCE_NAME"] = "Distance transform";
    translations["BLOCK__DISTANCE_IN_IMAGE"] = "image";
    translations["BLOCK__DISTANCE_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__DISTANCE_IN_DISTANCETYPE"] = "distance";
    translations["BLOCK__DISTANCE_IN_DISTANCETYPE_HELP"] = "distance type|DIST_C^DIST_L1^DIST_L2^DIST_PRECISE";
    translations["BLOCK__DISTANCE_OUT_IMAGE"] = "image";
    translations["BLOCK__DISTANCE_OUT_IMAGE_HELP"] = "Output image";

    translations["BLOCK__BINARIZE_NAME"] = "Binarize img";
    translations["BLOCK__BINARIZE_IN_IMAGE"] = "image";
    translations["BLOCK__BINARIZE_IN_IMAGE_HELP"] = "Input image";
    translations["BLOCK__BINARIZE_IN_INVERSE"] = "inverse";
    translations["BLOCK__BINARIZE_IN_INVERSE_HELP"] = "Use inverse of binary image";
    translations["BLOCK__BINARIZE_IN_METHOD"] = "Method";
    translations["BLOCK__BINARIZE_IN_METHOD_HELP"] = "Binarization method|Simple threshold^Otsu^Adaptative^Sauvola";
    translations["BLOCK__BINARIZE_IN_THRESHOLD"] = "threshold";
    translations["BLOCK__BINARIZE_IN_THRESHOLD_HELP"] = "Threshold value";
    translations["BLOCK__BINARIZE_OUT_IMAGE"] = "image";
    translations["BLOCK__BINARIZE_OUT_IMAGE_HELP"] = "Output image";

  }
}

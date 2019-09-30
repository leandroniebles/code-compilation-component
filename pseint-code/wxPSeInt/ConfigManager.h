#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <wx/string.h>
#include <wx/arrstr.h>
#include "../pseint/LangSettings.h"

#define _no_tty "<<sin configurar>>"

#define DEFAULT_PROFILE "Flexible"
#define CUSTOM_PROFILE "<Personalizado>"

class ConfigManager {
private:
	wxString tty_command;
public:
	bool use_psterm; ///< utilizar psterm en lugar de la terminal del sistema (recomendado)
	bool use_dark_theme; ///< esquema de colores para el resaltado de sintaxis, true=fondo negro, false=fondo blanco
	bool use_dark_psterm; ///< esquema de colores de psterm, true=fondo negro, false=fondo blanco
	wxString GetTTYCommand(); ///< terminal a usar si no se usa la propia (en GNU/Linux, hay que probar algunas para ver cual hay instalada)
	
	int version; ///< version del archivo de configuraci�n que se ley� al inicializar
	int comm_port;
	int debug_port;
	bool fixed_port;
	bool check_for_updates;
	wxString pseint_dir;
	wxString home_dir;
	wxString filename;
	wxArrayString last_files;
	wxString images_path;
	wxString pseint_command;
	wxString psterm_command;
	wxString psdrawe_command;
	wxString psdraw3_command;
	wxString psexport_command;
	wxString pseval_command;
	wxString last_dir;
	wxString temp_dir;
//	wxString /*temp*/_file;
	wxString help_dir;
	wxString examples_dir;
	wxString profiles_dir;
	wxString proxy;
	
private:
//	wxString profile;
	LangSettings lang;
public:
	bool LoadListedProfile(wxString name);
	bool LoadProfileFromFile(wxString path);
//	bool SetCustomProfile(LangSettings custom_lang);
	bool SetProfile(LangSettings a_lang);
//	wxString GetProfileName() const;
//	bool IsProfileListed() const;
	const LangSettings &GetLang() { return lang; }
	LangSettings &GetWritableLang() { return lang; }
	
	bool reorganize_for_debug; ///< si reacomoda o no las ventanas al lanzar la ejecuci�n paso a paso
	bool animate_gui; ///< si al mostrar u ocultar los paneles laterales lo hace con una animaci�n (true) o de forma instantanea (false)
	bool use_colors;
	bool colour_sintax;
	bool shape_colors; ///< usar diferentes colores de fondo para las distintas estructuras de control
	bool psdraw_nocrop; ///< no cortar labels largos en el diagrama (por defecto remplaza "xxxxxxxxxx" por "xxx...")
//	bool show_toolbar;
	bool show_vars;
	bool show_opers;
	bool show_commands;
	bool show_debug_panel;
	bool auto_quickhelp;
	bool autocomp;
	bool highlight_blocks;
	bool autoclose;
	bool calltip_helps;
	bool rt_syntax;
	bool rt_annotate;
	bool smart_indent;
	bool big_icons;
	long wx_font_size, term_font_size;
	wxString wx_font_name, term_font_name;
	int tabw;
	int size_x, size_y;
	int pos_x, pos_y;
	int stepstep_tspeed; ///< velocidad para la ejecucion paso a paso (posicion del scroll, no tiempo para el interprete)
	bool maximized;
	
	ConfigManager(wxString apath);
	void LoadDefaults();
	void Read();
	void Save();
	~ConfigManager();
	
	int GetCommPort();
	int GetDebugPort();
	
	void Log() const;
	
};

extern ConfigManager *config;
#define cfg_lang config->GetLang()

// para probar como seria la interfaz sin el result tree (usando el marcado de errores sobre el c�digo y el panel de ayuda r�pida para la informaci�n adicional)
#define _avoid_results_tree config->rt_syntax

#endif


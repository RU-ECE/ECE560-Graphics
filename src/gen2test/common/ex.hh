#pragma once
#include <iostream>
enum class error_type {
    out_of_memory = 0,
    invalid_argument = 1,
    invalid_value = 2,
    invalid_operation = 3,
    internal_error = 4,
    library_initialization = 5,
    file_not_found = 6,
    file_access_denied = 7,
    file_invalid_data = 8,
    file_not_enough_space = 9,
    file_already_exists = 10,
    file_not_a_directory = 11,
    file_not_a_regular_file = 12,
    initialization = 13, // general initialization for any application, with a string message with the name of the function called
    mainloop = 14, // general mainloop for any application, with a string message with the name of the function called
    cleanup = 15, // general cleanup for any application, with a string message with the name of the function called
    input = 16,    // input error, with a string message with the name of the function called
    out_of_bounds = 17
    
    //TODO: more errors here
};

struct error_mapping {
    error_type err;
    const char* en; // English
    const char* fr; // French
    const char* de; // German
    const char* it; // Italian
    const char* es; // Spanish
    const char* pt; // Portuguese
    const char* nl; // Dutch
    const char* uk; // Ukrainian
    const char* ru; // Russian
    const char* zh; // Chinese
    const char* ko; // Korean
    const char* ja; // Japanese
    const char* ar; // Arabic
    const char* hi; // Hindi
    const char* he; // Hebrew
    const char* tr; // Turkish
    
    constexpr error_mapping(error_type e, 
                          const char* en_str, const char* fr_str, const char* de_str,
                          const char* it_str, const char* es_str, const char* pt_str,
                          const char* nl_str, const char* uk_str, const char* ru_str,
                          const char* zh_str, const char* ko_str, const char* ja_str,
                          const char* ar_str, const char* hi_str, const char* he_str,
                          const char* tr_str)
        : err(e), en(en_str), fr(fr_str), de(de_str), it(it_str), es(es_str),
          pt(pt_str), nl(nl_str), uk(uk_str), ru(ru_str), zh(zh_str), ko(ko_str),
          ja(ja_str), ar(ar_str), hi(hi_str), he(he_str), tr(tr_str) {}
};

constexpr error_mapping error_strings[] = {
    {error_type::out_of_memory,
     "Out of memory",
     "Mémoire insuffisante",
     "Nicht genügend Arbeitsspeicher",
     "Memoria esaurita",
     "Memoria agotada",
     "Memória esgotada",
     "Onvoldoende geheugen",
     "Недостатньо пам'яті",
     "Недостаточно памяти",
     "内存不足",
     "메모리 부족",
     "メモリ不足",
     "نفدت الذاكرة",
     "मेमोरी कम पड़ गई",
     "אין מספיק זיכרון",
     "Yetersiz bellek"},
    {error_type::invalid_argument,
     "Invalid argument",
     "Argument invalide",
     "Ungültiges Argument",
     "Argomento non valido",
     "Argumento inválido",
     "Argumento inválido",
     "Ongeldig argument",
     "Недійсний аргумент",
     "Недопустимый аргумент",
     "无效参数",
     "잘못된 인수",
     "無効な引数",
     "وسيط غير صالح",
     "अमान्य तर्क",
     "ארגומנט לא חוקי",
     "Geçersiz argüman"},
    {error_type::invalid_value,
     "Invalid value",
     "Valeur invalide",
     "Ungültiger Wert",
     "Valore non valido",
     "Valor inválido",
     "Valor inválido",
     "Ongeldige waarde",
     "Недійсне значення",
     "Недопустимое значение",
     "无效值",
     "잘못된 값",
     "無効な値",
     "قيمة غير صالحة",
     "अमान्य मान",
     "ערך לא חוקי",
     "Geçersiz değer"},
    {error_type::invalid_operation,
     "Invalid operation",
     "Opération invalide",
     "Ungültige Operation",
     "Operazione non valida",
     "Operación inválida",
     "Operação inválida",
     "Ongeldige bewerking",
     "Недійсна операція",
     "Недопустимая операция",
     "无效操作",
     "잘못된 작업",
     "無効な操作",
     "عملية غير صالحة",
     "अमान्य संचालन",
     "פעולה לא חוקית",
     "Geçersiz işlem"},
    {error_type::internal_error,
     "Internal error",
     "Erreur interne",
     "Interner Fehler",
     "Errore interno",
     "Error interno",
     "Erro interno",
     "Interne fout",
     "Внутрішня помилка",
     "Внутренняя ошибка",
     "内部错误",
     "내부 오류",
     "内部エラー",
     "خطأ داخلي",
     "आंतरिक त्रुटि",
     "שגיאה פנימית",
     "İç hata"},
    {error_type::library_initialization,
     "Library initialization failed",
     "Échec de l'initialisation de la bibliothèque",
     "Bibliotheksinitialisierung fehlgeschlagen",
     "Inizializzazione della libreria fallita",
     "Fallo en la inicialización de la biblioteca",
     "Falha na inicialização da biblioteca",
     "Bibliotheekinitialisatie mislukt",
     "Помилка ініціалізації бібліотеки",
     "Ошибка инициализации библиотеки",
     "库初始化失败",
     "라이브러리 초기화 실패",
     "ライブラリの初期化に失敗",
     "فشل تهيئة المكتبة",
     "लाइब्रेरी प्रारंभीकरण विफल",
     "אתחול הספרייה נכשל",
     "Kütüphane başlatma başarısız"},
    {error_type::file_not_found,
     "File not found",
     "Fichier introuvable",
     "Datei nicht gefunden",
     "File non trovato",
     "Archivo no encontrado",
     "Arquivo não encontrado",
     "Bestand niet gevonden",
     "Файл не знайдено",
     "Файл не найден",
     "找不到文件",
     "파일을 찾을 수 없음",
     "ファイルが見つかりません",
     "الملف غير موجود",
     "फ़ाइल नहीं मिली",
     "הקובץ לא נמצא",
     "Dosya bulunamadı"},
    {error_type::file_access_denied,
     "File access denied",
     "Accès au fichier refusé",
     "Dateizugriff verweigert",
     "Accesso al file negato",
     "Acceso al archivo denegado",
     "Acesso ao arquivo negado",
     "Bestandstoegang geweigerd",
     "Доступ до файлу заборонено",
     "Доступ к файлу запрещен",
     "拒绝访问文件",
     "파일 접근 거부됨",
     "ファイルアクセスが拒否されました",
     "تم رفض الوصول إلى الملف",
     "फ़ाइल पहुंच अस्वीकृत",
     "הגישה לקובץ נדחתה",
     "Dosya erişimi reddedildi"},
    {error_type::file_invalid_data,
     "File contains invalid data",
     "Le fichier contient des données invalides",
     "Datei enthält ungültige Daten",
     "Il file contiene dati non validi",
     "El archivo contiene datos inválidos",
     "O arquivo contém dados inválidos",
     "Bestand bevat ongeldige gegevens",
     "Файл містить недійсні дані",
     "Файл содержит недопустимые данные",
     "文件包含无效数据",
     "파일에 잘못된 데이터가 있음",
     "ファイルに無効なデータが含まれています",
     "يحتوي الملف على بيانات غير صالحة",
     "फ़ाइल में अमान्य डेटा है",
     "הקובץ מכיל נתונים לא חוקיים",
     "Dosya geçersiz veriler içeriyor"},
    {error_type::file_not_enough_space,
     "Not enough space",
     "Espace insuffisant",
     "Nicht genügend Speicherplatz",
     "Spazio insufficiente",
     "Espacio insuficiente",
     "Espaço insuficiente",
     "Onvoldoende ruimte",
     "Недостатньо місця",
     "Недостаточно места",
     "空间不足",
     "공간이 부족함",
     "空き容量が足りません",
     "لا توجد مساحة كافية",
     "पर्याप्त स्थान नहीं है",
     "אין מספיק מקום",
     "Yeterli alan yok"},
    {error_type::file_already_exists,
     "File already exists",
     "Le fichier existe déjà",
     "Datei existiert bereits",
     "Il file esiste già",
     "El archivo ya existe",
     "O arquivo já existe",
     "Bestand bestaat al",
     "Файл вже існує",
     "Файл уже существует",
     "文件已存在",
     "파일이 이미 존재함",
     "ファイルは既に存在します",
     "الملف موجود بالفعل",
     "फ़ाइल पहले से मौजूद है",
     "הקובץ כבר קיים",
     "Dosya zaten var"},
    {error_type::file_not_a_directory,
     "Not a directory",
     "Ce n'est pas un répertoire",
     "Kein Verzeichnis",
     "Non è una directory",
     "No es un directorio",
     "Não é um diretório",
     "Geen map",
     "Не є каталогом",
     "Не является каталогом",
     "不是目录",
     "디렉터리가 아님",
     "ディレクトリではありません",
     "ليس دليلاً",
     "डायरेक्टरी नहीं है",
     "לא תיקייה",
     "Dizin değil"},
    {error_type::file_not_a_regular_file,
     "Not a regular file",
     "Ce n'est pas un fichier normal",
     "Keine reguläre Datei",
     "Non è un file regolare",
     "No es un archivo regular",
     "Não é um arquivo regular",
     "Geen gewoon bestand",
     "Не є звичайним файлом",
     "Не является обычным файлом",
     "不是常规文件",
     "일반 파일이 아님",
     "通常のファイルではありません",
     "ليس ملفاً عادياً",
     "नियमित फ़ाइल नहीं है",
     "לא קובץ רגיל",
     "Normal dosya değil"},
    {error_type::initialization,
     "Initialization failed",
     "Échec de l'initialisation",
     "Initialisierung fehlgeschlagen",
     "Inizializzazione fallita",
     "Fallo en la inicialización",
     "Falha na inicialização",
     "Initialisatie mislukt",
     "Помилка ініціалізації",
     "Ошибка инициализации",
     "初始化失败",
     "초기화 실패",
     "初期化に失敗しました",
     "فشل في التهيئة",
     "प्रारंभीकरण विफल",
     "האתחול נכשל",
     "Başlatma başarısız"},
    {error_type::mainloop,
     "Main loop error",
     "Erreur dans la boucle principale",
     "Fehler in der Hauptschleife",
     "Errore nel ciclo principale",
     "Error en el bucle principal",
     "Erro no loop principal",
     "Fout in hoofdlus",
     "Помилка в головному циклі",
     "Ошибка в главном цикле",
     "主循环错误",
     "메인 루프 오류",
     "メインループエラー",
     "خطأ في الحلقة الرئيسية",
     "मुख्य लूप त्रुटि",
     "שגיאה בלולאה הראשית",
     "Ana döngü hatası"},
    {error_type::cleanup,
     "Cleanup error",
     "Erreur de nettoyage",
     "Aufräumfehler",
     "Errore di pulizia",
     "Error de limpieza",
     "Erro na limpeza",
     "Opruimfout",
     "Помилка очищення",
     "Ошибка очистки",
     "清理错误",
     "정리 오류",
     "クリーンアップエラー",
     "خطأ في التنظيف",
     "क्लीनअप त्रुटि",
     "שגיאת ניקוי",
     "Temizleme hatası"},
    {error_type::input,
     "Input error",
     "Erreur d'entrée",
     "Eingabefehler",
     "Errore di input",
     "Error de entrada",
     "Erro de entrada",
     "Invoerfout",
     "Помилка введення",
     "Ошибка ввода",
     "输入错误",
     "입력 오류",
     "入力エラー",
     "خطأ في الإدخال",
     "इनपुट त्रुटि",
     "שגיאת קלט",
     "Giriş hatası"},
    {error_type::out_of_bounds,
     "Out of bounds",
     "Hors limites",
     "Außerhalb der Grenzen",
     "Fuori dai limiti",
     "Fuera de límites",
     "Fora dos limites",
     "Buiten bereik",
     "Поза межами",
     "Выход за пределы",
     "超出范围",
     "범위를 벗어남",
     "範囲外",
     "خارج النطاق",
     "सीमा से बाहर",
     "מחוץ לגבולות",
     "Sınırların dışında"}
};

enum class language {
    en,
    fr,
    de,
    it,
    es,
    pt,
    nl,
    uk,
    ru,
    zh,
    ko,
    ja,
    ar,
    hi,
    he,
    tr
};

extern language current_lang;

inline void set_language(language lang)  {
    current_lang = lang;
}

class ex {
public:
    const char* srcfile;
    uint32_t line;
    error_type err;
    const char* strparam;

    ex(const char* srcfile, uint32_t line, error_type err, const char* strparam = nullptr) : 
    srcfile(srcfile), line(line), err(err), strparam(strparam) {}
    friend std::ostream& operator<<(std::ostream& os, const ex& e) {
        os << e.srcfile << ":" << e.line << " ";
        const error_mapping& mapping = error_strings[static_cast<size_t>(e.err)];
        // Select the correct language string based on current_lang
        switch (current_lang) {
            case language::en: os << mapping.en; break;
            case language::fr: os << mapping.fr; break;
            case language::de: os << mapping.de; break;
            case language::it: os << mapping.it; break;
            case language::es: os << mapping.es; break;
            case language::pt: os << mapping.pt; break;
            case language::nl: os << mapping.nl; break;
            case language::uk: os << mapping.uk; break;
            case language::ru: os << mapping.ru; break;
            case language::zh: os << mapping.zh; break;
            case language::ko: os << mapping.ko; break;
            case language::ja: os << mapping.ja; break;
            case language::ar: os << mapping.ar; break;
            case language::hi: os << mapping.hi; break;
            case language::he: os << mapping.he; break;
            case language::tr: os << mapping.tr; break;
        }
        if (e.strparam) os << " " << e.strparam;
        return os;
    }
};

extern uint32_t debug_level;

inline const char* get_localized_string(const error_mapping& mapping) {
    switch (current_lang) {
        case language::en: return mapping.en;
        case language::fr: return mapping.fr;
        case language::de: return mapping.de;
        case language::it: return mapping.it;
        case language::es: return mapping.es;
        case language::pt: return mapping.pt;
        case language::nl: return mapping.nl;
        case language::uk: return mapping.uk;
        case language::ru: return mapping.ru;
        case language::zh: return mapping.zh;
        case language::ko: return mapping.ko;
        case language::ja: return mapping.ja;
        case language::ar: return mapping.ar;
        case language::hi: return mapping.hi;
        case language::he: return mapping.he;
        case language::tr: return mapping.tr;
    }
    return mapping.en; // fallback to English
}

inline void log(const char file[], uint32_t line, 
                error_type err, const char* msg) {
    if (debug_level > 0) {
        const char* msg_str = get_localized_string(error_strings[static_cast<size_t>(err)]);
        std::cout << file << ":" << line << " " << msg_str << " " << msg << std::endl;
    }
}

inline void log(const char file[], uint32_t line, 
                error_type err, const char* msg, uint32_t param) {
    if (debug_level > 0) {
        const char* msg_str = get_localized_string(error_strings[static_cast<size_t>(err)]);
        std::cout << file << ":" << line << " " << msg_str << " " << msg << " " << param << std::endl;
    }
}

#define vkcheck(result, err) \
if (result != VK_SUCCESS) \
    throw ex(__FILE__, __LINE__, err);

#define vkcheck_msg(result, err, msg) \
if (result != VK_SUCCESS) \
    throw ex(__FILE__, __LINE__, err, msg);


#define oglcheck_msg(result, errcode, msg) \
if (!result) { \
    throw ex(__FILE__, __LINE__, errcode, msg); \
} else { \
    if (debug_level > 0) { \
        log(__FILE__, __LINE__, errcode, msg); \
    }\
}

import os
import sys

# -----------------------------
# ⚙️ CONFIGURACIÓN DE EXCLUSIÓN
# -----------------------------

EXCLUIR_NOMBRES = {
    ".git",
    ".gitignore",
    "__pycache__",
    ".DS_Store",
    "venv",
    ".venv",
    "node_modules"
}

EXCLUIR_EXTENSIONES = {
    ".pyc",
    ".log",
    ".tmp",
    ".swp"
}


def debe_excluir(nombre):
    # Excluir por nombre exacto
    if nombre in EXCLUIR_NOMBRES:
        return True

    # Excluir archivos ocultos (opcional)
    if nombre.startswith("."):
        return True

    # Excluir por extensión
    _, extension = os.path.splitext(nombre)
    if extension in EXCLUIR_EXTENSIONES:
        return True

    return False


def imprimir_arbol(ruta, prefijo=""):
    try:
        elementos = sorted(os.listdir(ruta))
    except PermissionError:
        print(prefijo + "└── [Acceso denegado]")
        return

    # Filtrar elementos
    elementos = [e for e in elementos if not debe_excluir(e)]

    for i, elemento in enumerate(elementos):
        ruta_completa = os.path.join(ruta, elemento)
        es_ultimo = i == len(elementos) - 1

        conector = "└── " if es_ultimo else "├── "
        print(prefijo + conector + elemento)

        if os.path.isdir(ruta_completa):
            extension = "    " if es_ultimo else "│   "
            imprimir_arbol(ruta_completa, prefijo + extension)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        RUTA_BASE = sys.argv[1]
    else:
        RUTA_BASE = os.getcwd()

    print(f"\n📂 Árbol de directorios para: {RUTA_BASE}\n")
    imprimir_arbol(RUTA_BASE) 
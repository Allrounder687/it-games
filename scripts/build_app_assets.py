import os
import json
from PIL import Image, ImageDraw, ImageFont

def generate_assets(output_dir):
    sce_sys_dir = os.path.join(output_dir, "sce_sys")
    os.makedirs(sce_sys_dir, exist_ok=True)

    # 1. Generate param.json for PS5
    param_json = {
        "applicationCategoryType": 0,
        "applicationDrmType": "upgradable",
        "attribute": 0,
        "attribute2": 0,
        "attribute3": 0,
        "conceptId": "999001",
        "contentBadgeType": 1,
        "contentId": "IV0001-ITGA00001_00-ITGAMES000000000",
        "contentVersion": "01.000.000",
        "downloadDataSize": 0,
        "localizedParameters": {
            "defaultLanguage": "en-US",
            "en-US": {
                "titleName": "IT Games"
            }
        },
        "masterVersion": "01.00",
        "pubtools": {
            "creationDate": "2026-08-19 23:40:00",
            "toolVersion": "1.00"
        },
        "requiredSystemSoftwareVersion": "0x0100000000000000",
        "sdkVersion": "0x0100000000000000",
        "titleId": "ITGA00001"
    }

    param_path = os.path.join(sce_sys_dir, "param.json")
    with open(param_path, "w", encoding="utf-8") as f:
        json.dump(param_json, f, indent=2)
    print(f"Generated: {param_path}")

    # 2. Generate icon0.png (512x512)
    icon_size = (512, 512)
    icon = Image.new("RGBA", icon_size, (15, 18, 30, 255))
    draw = ImageDraw.Draw(icon)

    # Gradient circle background
    for r in range(230, 0, -1):
        color = (
            int(30 + (225 - 30) * (230 - r) / 230 * 0.2),
            int(80 + (240 - 80) * (230 - r) / 230 * 0.4),
            int(220 + (255 - 220) * (230 - r) / 230 * 0.6),
            255
        )
        draw.ellipse([256 - r, 256 - r, 256 + r, 256 + r], fill=color)

    # Inner badge
    draw.rounded_rectangle([90, 140, 422, 372], radius=32, fill=(20, 25, 45, 240), outline=(0, 200, 255, 255), width=4)

    # Text
    draw.text((256, 210), "IT GAMES", fill=(255, 255, 255), anchor="mm")
    draw.text((256, 260), "PS5 HOMEBREW", fill=(0, 210, 255), anchor="mm")
    draw.text((256, 310), "v1.0.0", fill=(160, 180, 210), anchor="mm")

    icon_path = os.path.join(sce_sys_dir, "icon0.png")
    icon.save(icon_path, "PNG")
    print(f"Generated: {icon_path}")

    # 3. Generate pic0.png / pic1.png (1920x1080 Splash)
    pic_size = (1920, 1080)
    pic = Image.new("RGBA", pic_size, (10, 12, 22, 255))
    pic_draw = ImageDraw.Draw(pic)

    # Background ambient glows
    pic_draw.ellipse([700, 300, 1220, 820], fill=(20, 50, 120, 255))
    pic_draw.rounded_rectangle([560, 340, 1360, 740], radius=40, fill=(18, 22, 38, 230), outline=(0, 180, 255, 255), width=5)

    pic_draw.text((960, 480), "IT GAMES", fill=(255, 255, 255), anchor="mm")
    pic_draw.text((960, 560), "Native PlayStation 5 Homebrew Application", fill=(0, 200, 255), anchor="mm")
    pic_draw.text((960, 630), "Standalone fPKG Architecture", fill=(160, 180, 210), anchor="mm")

    pic0_path = os.path.join(sce_sys_dir, "pic0.png")
    pic.save(pic0_path, "PNG")
    print(f"Generated: {pic0_path}")

    pic1_path = os.path.join(sce_sys_dir, "pic1.png")
    pic.save(pic1_path, "PNG")
    print(f"Generated: {pic1_path}")

if __name__ == "__main__":
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else "app_pkg/ITGA00001-app0"
    generate_assets(target)
